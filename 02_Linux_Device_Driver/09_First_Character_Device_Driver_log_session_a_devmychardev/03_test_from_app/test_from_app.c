#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char buffer[100];
    ssize_t bytes_read;

    fd = open("/dev/mychardev", O_RDWR);
    if (fd < 0) {
        perror("Failed to open /dev/mychardev");
        return 1;
    }

    const char *msg = "Testing from app";
    if (write(fd, msg, strlen(msg)) < 0) {
        perror("Failed to write to device");
        close(fd);
        return 1;
    }

    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from device");
        close(fd);
        return 1;
    }

    buffer[bytes_read] = '\0';
    printf("Read from device: %s\n", buffer);

    close(fd);
    return 0;
}
