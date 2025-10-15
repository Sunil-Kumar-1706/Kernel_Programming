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
	unsigned char ch = 'A';
	int i = 0;

	fd = open("/dev/msg", O_RDWR);
	if (fd < 0) {
		perror("fd failed");
		exit(2);
	}


	ioctl(fd, MSG_GET_ADDRESS, &ch);
	perror("ioctl");
	getchar();
	printf("%x\n", ch);

	close(fd);
}
