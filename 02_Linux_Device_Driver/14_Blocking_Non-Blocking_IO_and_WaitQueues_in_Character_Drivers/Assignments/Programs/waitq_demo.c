#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE "/dev/waitqdev"
#define BUF_SIZE 256

int fd;

void* reader_thread(void* arg) {
    char buf[BUF_SIZE] = {0};
    printf("[Reader] Waiting to read from device...\n");

    ssize_t n = read(fd, buf, BUF_SIZE - 1);
    if (n < 0) {
        perror("[Reader] read failed");
        pthread_exit(NULL);
    }

    buf[n] = '\0';
    printf("[Reader] Read: '%s'\n", buf);
    pthread_exit(NULL);
}

void* writer_thread(void* arg) {
    const char* msg = "Hello from writer!";
    printf("[Writer] Sleeping for 3 seconds before writing...\n");
    sleep(3);

    ssize_t n = write(fd, msg, strlen(msg));
    if (n < 0) {
        perror("[Writer] write failed");
        pthread_exit(NULL);
    }

    printf("[Writer] Wrote: '%s'\n", msg);
    pthread_exit(NULL);
}

int main() {
    pthread_t reader, writer;

    fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }
    printf("[Main] Device opened: %s\n", DEVICE);

    pthread_create(&reader, NULL, reader_thread, NULL);
    pthread_create(&writer, NULL, writer_thread, NULL);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    close(fd);
    printf("[Main] Device closed.\n");

    return EXIT_SUCCESS;
}
