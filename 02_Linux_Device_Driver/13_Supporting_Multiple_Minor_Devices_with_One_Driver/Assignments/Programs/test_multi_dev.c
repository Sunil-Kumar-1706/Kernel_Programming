#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define NUM_DEVICES 3
#define BUFFER_SIZE 256

int main() {
    int fds[NUM_DEVICES];
    char device_name[20];
    char read_buf[BUFFER_SIZE];
    int i;

    for (i = 0; i < NUM_DEVICES; i++) {
        snprintf(device_name, sizeof(device_name), "/dev/mydev%d", i);
        fds[i] = open(device_name, O_RDWR);
        if (fds[i] < 0) {
            perror(device_name);
            return 1;
        }
    }

    for (i = 0; i < NUM_DEVICES; i++) {
        snprintf(read_buf, sizeof(read_buf), "Message for device %d", i);
        write(fds[i], read_buf, strlen(read_buf));
        printf("Wrote to %s: %s\n", device_name, read_buf);
    }

    for (i = 0; i < NUM_DEVICES; i++) {
        lseek(fds[i], 0, SEEK_SET);
        int n = read(fds[i], read_buf, sizeof(read_buf)-1);
        if (n > 0) {
            read_buf[n] = '\0';
            printf("Read from /dev/mydev%d: %s\n", i, read_buf);
        }
    }

    for (i = 0; i < NUM_DEVICES; i++)
        close(fds[i]);

    return 0;
}
