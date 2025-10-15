#include <stdio.h>
#include <sys/fcntl.h>

int main()
{
	int fd;

	fd = open("/etc/shadow", O_RDONLY);
	perror("fd");
	return 0;
}
