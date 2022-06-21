#include "video_device.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include "util.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>

DeviceInfo::DeviceInfo(const char *name): mDeviceName(name), mfd(-1),
            mBuffCount(4), frame_buffers(nullptr),
            mWidth(1920), mHeight(1080)
{
}

DeviceInfo::~DeviceInfo()
{}

int DeviceInfo::open_device() {
    struct stat st;
    if(-1 == stat(mDeviceName.c_str(), &st)) {
        LOGE("Cannot identify '%s': %d, %s", mDeviceName.c_str(), errno, strerror(errno));
        return ERROR_LOCAL;
    }

    if(!S_ISCHR(st.st_mode)) {
        LOGE("%s is not a valid device", mDeviceName.c_str());
        return ERROR_LOCAL;
    }

    mfd = open(mDeviceName.c_str(), O_RDWR | O_NONBLOCK, 0);
    if(-1 == mfd) {
        LOGE("Cannot open '%s': %d, %s", mDeviceName.c_str(), errno, strerror(errno));
        if(EACCES == errno) {
            LOGE("Insufficient permissions on '%s': %d, %s", mDeviceName.c_str(), errno,
                    strerror(errno));
        }
        return ERROR_LOCAL;
    }

    return SUCCESS_LOCAL;
}

int DeviceInfo::init_mmap() {
    struct v4l2_requestbuffers req;
    CLEAR(req);
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    req.memory = V4L2_MEMORY_MMAP;

    if(-1 == xioctl(mfd, VIDIOC_REQBUFS, &req)) {
        if(EINVAL == errno) {
            LOGE("device does not support memory mapping");
            return ERROR_LOCAL;
        } else {
            return errnoexit("VIDIOC_REQBUFS");
        }
    }

    if(req.count < 2) {
        LOGE("Insufficient buffer memory");
        return ERROR_LOCAL;
    }
    if (frame_buffers == nullptr) {
       // frame_buffers = calloc(req.count, sizeof(*frame_buffers));
        frame_buffers = new struct buffer[req.count];
        if(!frame_buffers) {
            LOGE("Out of memory");
            return ERROR_LOCAL;
        }
    }


    for(mBuffCount = 0; mBuffCount < req.count; ++mBuffCount) {
        struct v4l2_buffer buf;
        struct v4l2_plane planes[1];
        CLEAR(buf);
        CLEAR(planes);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = mBuffCount;
        buf.m.planes = planes;
        buf.length = 1;

        if(-1 == xioctl(mfd, VIDIOC_QUERYBUF, &buf)) {
            return errnoexit("VIDIOC_QUERYBUF");
        }

        frame_buffers[mBuffCount].length = buf.m.planes[0].length;
        frame_buffers[mBuffCount].start = mmap(NULL /* start anywhere */,
                              buf.m.planes[0].length,
                              PROT_READ | PROT_WRITE /* required */,
                              MAP_SHARED /* recommended */,
                              mfd, buf.m.planes[0].m.mem_offset);

        if(MAP_FAILED == frame_buffers[mBuffCount].start) {
            return errnoexit("mmap");
        }
    }

    return SUCCESS_LOCAL;
}

int DeviceInfo::init_device(int width, int height) {
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    unsigned int min;
    mWidth = width;
    mHeight = height;

    if(-1 == xioctl(mfd, VIDIOC_QUERYCAP, &cap)) {
        if(EINVAL == errno) {
            LOGE("not a valid V4L2 device");
            return ERROR_LOCAL;
        } else {
            return errnoexit("VIDIOC_QUERYCAP");
        }
    }

    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) && 
        !(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE)) {
        LOGE("device is not a video capture device, capabilities:0x%x", cap.capabilities);
        return ERROR_LOCAL;
    }

    if(!(cap.capabilities & V4L2_CAP_STREAMING)) {
        LOGE("device does not support streaming i/o");
        return ERROR_LOCAL;
    }

    CLEAR(cropcap);
    if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) {
        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    }
    else if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE) {
        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    }
    

    if(0 == xioctl(mfd, VIDIOC_CROPCAP, &cropcap)) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        crop.c = cropcap.defrect;

        if(-1 == xioctl(mfd, VIDIOC_S_CROP, &crop)) {
            switch(errno) {
                case EINVAL:
                    break;
                default:
                    break;
            }
        }
    }

    CLEAR(fmt);
    if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) {
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    }
    else if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE) {
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    }
    
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if(-1 == xioctl(mfd, VIDIOC_S_FMT, &fmt)) {
        return errnoexit("VIDIOC_S_FMT");
    }

    min = fmt.fmt.pix.width * 2;
    if(fmt.fmt.pix.bytesperline < min) {
        fmt.fmt.pix.bytesperline = min;
    }

    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if(fmt.fmt.pix.sizeimage < min) {
        fmt.fmt.pix.sizeimage = min;
    }

    return init_mmap();
}

int DeviceInfo::uninit_device() {
    for(unsigned int i = 0; i < mBuffCount; ++i) {
        if(-1 == munmap(frame_buffers[i].start, frame_buffers[i].length)) {
            return errnoexit("munmap");
        }
    }

    delete[] frame_buffers;
    return SUCCESS_LOCAL;
}

int DeviceInfo::close_device() {
    int result = SUCCESS_LOCAL;
    if(-1 != mfd && -1 == close(mfd)) {
        result = errnoexit("close");
    }
    mfd = -1;
    return result;
}

