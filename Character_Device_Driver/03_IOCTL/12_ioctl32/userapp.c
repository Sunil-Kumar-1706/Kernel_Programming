#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <sys/ioctl.h>
#include "ioctl_cmd.h"

int main(int argc, char *argv[])
{
	char buffer[1024];	
	int fd;
	unsigned int length;
	int i = 0;

	fd = open("/dev/msg", O_RDWR);
	if (fd < 0) {
		perror("fd failed");
		exit(2);
	}


	ioctl(fd, MSG_IOCTL_GET_LENGTH, &length);
	perror("ioctl");
	getchar();
	printf("length: %d\n", length);

	close(fd);
}
