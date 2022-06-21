
#include "yuv.h"
#include "util.h"
#include "libyuv.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void uyvy422_to_argb(unsigned char *src, int width, int height, int *rgb_buffer)
{   
    int src_stride_uyvy = width * 2;
    int dst_stride_argb = width * 4;
    if((!rgb_buffer)) {
        return;
    }
    int *bgr_buffer = (int*)malloc(sizeof(int) * width * height);
    libyuv::UYVYToARGB(src, src_stride_uyvy, (unsigned char *)bgr_buffer, dst_stride_argb, width, height);
    //ARGBToBGRA(bgr_buffer, dst_stride_argb, rgb_buffer, dst_stride_argb, width, height);
    libyuv::ARGBToABGR((unsigned char *)bgr_buffer, dst_stride_argb, (unsigned char *)rgb_buffer, dst_stride_argb, width, height);
    free(bgr_buffer);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */