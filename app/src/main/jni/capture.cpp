#include "capture.h"

#include "util.h"
#include "yuv.h"

#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/videodev2.h>

Capture::Capture(const char *name):DeviceInfo(name), rgb_buffer(nullptr), y_buffer(nullptr)
{
}

Capture::~Capture()
{
}


int Capture::start_capture() {
    unsigned int i;
    enum v4l2_buf_type type;

    for(i = 0; i < mBuffCount; ++i) {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        struct v4l2_plane planes[1];
        buf.m.planes = planes;
        buf.length = 1;

        if(-1 == xioctl(mfd, VIDIOC_QBUF, &buf)) {
            return errnoexit("VIDIOC_QBUF");
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    if(-1 == xioctl(mfd, VIDIOC_STREAMON, &type)) {
        return errnoexit("VIDIOC_STREAMON");
    }

    int area = mWidth * mHeight;
    if (rgb_buffer == nullptr) 
        rgb_buffer = new int[area];
    if (y_buffer == nullptr) 
        y_buffer = new int[area];

    if (rgb_buffer == nullptr || y_buffer == nullptr) {
        return errnoexit("Malloc RGB buffer");
    }

    return SUCCESS_LOCAL;
}

int Capture::read_frame(int width, int height)
{
    struct v4l2_buffer buf;
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == buf.type) {
            struct v4l2_plane planes[1];
            buf.m.planes = planes;
            buf.length = 1;
        }

    if(-1 == xioctl(mfd, VIDIOC_DQBUF, &buf)) {
        switch(errno) {
            case EAGAIN:
                return 0;
            case EIO:
            default:
                return errnoexit("VIDIOC_DQBUF");
        }
    }

    assert(buf.index < mBuffCount);
   /* yuyv422_to_argb(frame_buffers[buf.index].start, width, height, rgb_buffer,
            y_buffer); */
    uyvy422_to_argb((unsigned char *)frame_buffers[buf.index].start, width, height, rgb_buffer);

    if(-1 == xioctl(mfd, VIDIOC_QBUF, &buf)) {
        return errnoexit("VIDIOC_QBUF");
    }

    return 1;
}

int Capture::stop_capturing()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    if(-1 != mfd && -1 == xioctl(mfd, VIDIOC_STREAMOFF, &type)) {
        return errnoexit("VIDIOC_STREAMOFF");
    }

    return SUCCESS_LOCAL;
}

void Capture::process_camera(int width, int height) {
    if(mfd == -1) {
        errnoexit("Invalid handle.....");
        return;
    }

    for(;;) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(mfd, &fds);

        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        int result = select(mfd + 1, &fds, NULL, NULL, &tv);
        if(-1 == result) {
            if(EINTR == errno) {
                continue;
            }
            errnoexit("select");
        } else if(0 == result) {
            LOGE("select timeout");
        }

        if(read_frame(width, height) == 1) {
            break;
        }
    }
}

void Capture::stop_camera() {
    stop_capturing();
    uninit_device();
    close_device();

    if(rgb_buffer) {
        delete[] rgb_buffer;
    }

    if(y_buffer) {
        delete[] y_buffer;
    }
}
