#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#define DEVICE "/dev/ioctldev"
#define MAJOR_NUM 240
#define IOCTL_RESET_BUFFER _IO(MAJOR_NUM, 0)
#define IOCTL_GET_COUNT    _IOR(MAJOR_NUM, 1, int)
#define IOCTL_SET_DEBUG    _IOW(MAJOR_NUM, 2, int)

int main() {
    int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char msg[] = "Hello ioctl!";
    write(fd, msg, strlen(msg));
    printf("Wrote: %s\n", msg);

    int count;
    ioctl(fd, IOCTL_GET_COUNT, &count);
    printf("Write count = %d\n", count);

    ioctl(fd, IOCTL_RESET_BUFFER);
    printf("Buffer reset.\n");

    int debug_flag = 1;
    ioctl(fd, IOCTL_SET_DEBUG, &debug_flag);
    printf("Debug mode enabled.\n");

    char debug_msg[] = "Debug message test!";
    write(fd, debug_msg, strlen(debug_msg));
    printf("Wrote: %s\n", debug_msg);

    debug_flag = 0;
    ioctl(fd, IOCTL_SET_DEBUG, &debug_flag);
    printf("Debug mode disabled.\n");

    char normal_msg[] = "Normal message";
    write(fd, normal_msg, strlen(normal_msg));
    printf("Wrote: %s\n", normal_msg);

    close(fd);
    return 0;
}
