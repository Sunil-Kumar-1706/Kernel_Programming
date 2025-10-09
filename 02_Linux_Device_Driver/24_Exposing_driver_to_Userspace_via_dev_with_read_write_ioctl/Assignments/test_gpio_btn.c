#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define GPIOBTN_RESET _IO('G', 0)

int main() {
    int fd = open("/dev/gpiobtn", O_RDWR);
    if (fd < 0) return 1;

    char buf[64];
    read(fd, buf, sizeof(buf));
    printf("Read: %s", buf);

    write(fd, "1", 1);  // toggle LED
    ioctl(fd, GPIOBTN_RESET);  


 
    close(fd);
    return 0;
}
