#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <stdint.h>

int main()
{
	int fd;
	uint64_t size;

	fd = open("/dev/sda1", O_RDONLY);
	perror("fd");

	ioctl(fd, BLKGETSIZE64,	&size);
	perror("ioctl");
	printf("Size %ld GB\n",  (size)/ (1024 * 1024 * 1024));
	close(fd);
	return 0;
}
