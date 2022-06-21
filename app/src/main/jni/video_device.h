#ifndef __VIDEO_DEVICE_H__
#define __VIDEO_DEVICE_H__

#include <jni.h>
#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <map>

class DeviceInfo {
public:
    struct buffer{
        void* start;
        size_t length;
    };


public:
    explicit DeviceInfo(const char *name);
    virtual ~DeviceInfo();

    /* Private: Open the video device at the named device node.
    *
    * Returns SUCCESS_LOCAL if the device was found and opened and ERROR_LOCAL if
    * an error occurred.
    */
    int open_device();

    /* Private: Initialize memory mapped buffers for video frames.
    *
    * fd - a valid file descriptor pointing to the camera device.
    *
    * Returns SUCCESS_LOCAL if no errors, otherwise ERROR_LOCAL.
    */
    int init_mmap();

    /* Private: Initialize video device with the given frame size.
    *
    * Initializes the device as a video capture device (must support V4L2) and
    * checks to make sure it has the streaming I/O interface. Configures the device
    * to crop the image to the given dimensions and initailizes a memory mapped
    * frame buffer.
    *
    * width - the desired width for the output images.
    * height - the desired height for the output images.
    *
    * Returns SUCCESS_LOCAL if no errors, otherwise ERROR_LOCAL.
    */
    int init_device(int width, int height);

    /* Private: Unmap and free memory-mapped frame buffers from the device.
    *
    * Returns SUCCESS_LOCAL if no errors, otherwise ERROR_LOCAL.
    */
    int uninit_device();

    /* Private: Close a file descriptor.
    *
    *
    * Returns SUCCESS_LOCAL if no errors, otherwise ERROR_LOCAL.
    */
    int close_device();

protected:
    int mfd;
    int mWidth;
    int mHeight;
    int mBuffCount;
    std::string mDeviceName;
    struct buffer *frame_buffers;
    //typedef std::map<int, string> DeviceMap;
};

#endif // __VIDEO_DEVICE_H__
