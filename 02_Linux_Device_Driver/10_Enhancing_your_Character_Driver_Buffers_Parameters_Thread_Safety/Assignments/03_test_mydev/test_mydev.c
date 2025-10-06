#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE_LEN 100
#define MAX_READ 1024

int main() {
    int fd;
    char *lines[] = {
        "Message 1 from app",
        "Message 2 from app",
        "Message 3 from app"
    };
    int num_lines = sizeof(lines) / sizeof(lines[0]);
    char buffer[MAX_READ];
    ssize_t bytes_read;
    int i;

    fd = open("/dev/mydev", O_RDWR);
    if (fd < 0) {
        perror("Failed to open /dev/mydev");
        return 1;
    }

    for (i = 0; i < num_lines; i++) {
        if (write(fd, lines[i], strlen(lines[i])) < 0) {
            perror("Failed to write to device");
            close(fd);
            return 1;
        }
        printf("Wrote: %s\n", lines[i]);
    }

    printf("\nReading messages back:\n");

  for (i = 0; i < num_lines; i++) {
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("%s\n", buffer);
    }
}
    if (bytes_read < 0) {
        perror("Failed to read from device");
    }

    close(fd);
    return 0;
}
