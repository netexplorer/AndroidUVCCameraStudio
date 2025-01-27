#include "util.h"

#include <errno.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int errnoexit(const char *s) {
    LOGE("%s error %d, %s", s, errno, strerror(errno));
    return ERROR_LOCAL;
}

int xioctl(int fd, int request, void *arg) {
    int r;

    do {
        r = ioctl(fd, request, arg);
    } while(-1 == r && EINTR == errno);

    return r;
}
#ifdef __cplusplus
}
#endif /* __cplusplus */