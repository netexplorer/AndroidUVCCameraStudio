#ifndef __YUV__H__
#define __YUV__H__

#include <jni.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* Private: Convert an UYVY422 image to an ARGB image.
 *
 * src - the source image buffer.
 * width - the width of the image.
 * height - the height of the image.
 * rgb_buffer - output buffer for RGB data from the conversion.
 */
void uyvy422_to_argb(unsigned char *src, int width, int height, int *rgb_buffer);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // __YUV__H__
