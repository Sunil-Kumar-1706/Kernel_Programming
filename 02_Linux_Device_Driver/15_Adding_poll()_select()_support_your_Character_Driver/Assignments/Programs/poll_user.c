#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>

#define DEVICE "/dev/polldev"
#define BUF_SIZE 256

int main() {
    int fd = open(DEVICE, O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN | POLLOUT;

    printf("Waiting for read or write readiness (poll)...\n");

    int ret = poll(&pfd, 1, 10000); // 10-second timeout
    if (ret == 0) {
        printf("Timeout! No events.\n");
    } else {
        if (pfd.revents & POLLIN) {
            char buf[BUF_SIZE];
            int n = read(fd, buf, sizeof(buf)-1);
            if (n < 0) perror("read");
            else {
                buf[n] = '\0';
                printf("Ready to read: %s\n", buf);
            }
        }

        if (pfd.revents & POLLOUT) {
            const char *msg = "Hello from user program!";
            int n = write(fd, msg, strlen(msg));
            if (n < 0) perror("write");
            else printf("Ready to write, wrote: %s\n", msg);
        }
    }

    close(fd);
    return 0;
}
