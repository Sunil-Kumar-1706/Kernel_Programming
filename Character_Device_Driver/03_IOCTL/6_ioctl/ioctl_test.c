#include <stdio.h>
#include <linux/ioctl.h>

#define MAGIC_NUMBER1	'l'
#define MAGIC_NUMBER2	'i'

int main()
{
	printf("IO(MAGIC_NUMBER1, 1):%x\n", _IO(MAGIC_NUMBER1, 1));
	printf("IO(MAGIC_NUMBER2, 1):%x\n", _IO(MAGIC_NUMBER2, 1));

	getchar();
	printf("IO(MAGIC_NUMBER1, 2):%x\n", _IO(MAGIC_NUMBER1, 2));
	printf("IO(MAGIC_NUMBER2, 2):%x\n", _IO(MAGIC_NUMBER2, 2));
	
	getchar();
	printf("IOR(MAGIC_NUMBER1, 2, int):%lx\n", _IOR(MAGIC_NUMBER1, 2, int));
	printf("IOR(MAGIC_NUMBER2, 2, int):%lx\n", _IOR(MAGIC_NUMBER2, 2, int));

	getchar();

	printf("IOR(MAGIC_NUMBER1, 3, int):%lx\n", _IOR(MAGIC_NUMBER1, 3, int));
	printf("IOR(MAGIC_NUMBER2, 3, int):%lx\n", _IOR(MAGIC_NUMBER2, 3, int));

	getchar();
	printf("IOWR(MAGIC_NUMBER2, 3, int):%lx\n", _IOWR(MAGIC_NUMBER2, 4, int));
	return 0;

}
