#include <stdio.h>
#include <linux/ioctl.h>

#define MAGIC_NUMBER1	'l'

#define CMD1 _IO(MAGIC_NUMBER1, 1)
#define CMD2 _IOW(MAGIC_NUMBER1, 2, int)
#define CMD3 _IOWR(MAGIC_NUMBER1, 3, char)

void printDirection(unsigned int cmd)
{
	switch(_IOC_DIR(cmd))
	{
		case _IOC_NONE:
			printf("_IOC_NONE\n");
			break;
		case _IOC_READ:
			printf("_IOC_READ\n");
			break;
		case _IOC_WRITE:
			printf("_IOC_WRITE\n");
			break;
		case _IOC_READ | _IOC_WRITE:
			printf("_IOC_READ | _IOC_WRITE\n");
			break;
	}
}

int main()
{
	printf("Number:%u\n", _IOC_NR(CMD1));
	printf("Number:%lu\n", _IOC_NR(CMD2));
	printf("Number:%lu\n", _IOC_NR(CMD3));

	printf("Magic Number:%u\n", _IOC_TYPE(CMD1));
	printf("Magic Number:%lu\n", _IOC_TYPE(CMD2));
	printf("Magic Number:%lu\n", _IOC_TYPE(CMD3));

	printf("Size:%u\n", _IOC_SIZE(CMD1));
	printf("Size:%lu\n", _IOC_SIZE(CMD2));
	printf("Size:%lu\n", _IOC_SIZE(CMD3));
	
	printDirection(CMD1);
	printDirection(CMD2);
	printDirection(CMD3);

	return 0;

}
