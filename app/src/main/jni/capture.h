#ifndef __CAPTURE_H__
#define __CAPTURE_H__

#include <jni.h>
#include <stdio.h>

#include "video_device.h"

class Capture : public DeviceInfo {
public:
        explicit Capture(const char *name);
        virtual ~Capture();
        
        int *get_rgb_buffer() {return rgb_buffer;}

        int *get_y_buffer() {return y_buffer;}

        bool isOpened() {return mfd != -1;}

        /* Private: Begins capturing video frames from a previously initialized device.
        *
        * The buffers in FRAME_BUFFERS are handed off to the device.
        *
        * Returns SUCCESS_LOCAL if no errors, otherwise ERROR_LOCAL.
        */
        int start_capture();

        /* Private: Read a single frame of video from the device into a buffer.
        *
        * The resulting image is stored in RGBA format across two buffers, rgb_buffer
        * and y_buffer.
        *
        * width - the width of the image.
        * height - the height of the image.
        *
        * Returns SUCCESS_LOCAL if no errors, otherwise ERROR_LOCAL.
        */
        int read_frame(int width, int height);

        /* Private: Unconfigure the video device for capturing.
        *
        * Returns SUCCESS_LOCAL if no errors, otherwise ERROR_LOCAL.
        */
        int stop_capturing();

        /* Private: Request a frame of video from the device to be output into the rgb
        * and y buffers.
        *
        * If the descriptor is not valid, no frame will be read.
        *
        * width - the width of the image.
        * height - the height of the image.
        */
        void process_camera(int width, int height);

        /* Private: Stop capturing, uninitialize the device and free all memory. */
        void stop_camera();


private:
        int* rgb_buffer;
        int* y_buffer;

};


#endif // __CAPTURE_H__
