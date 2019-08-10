#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd = open("/dev/ledDev", O_RDWR);
//	write(fd, NULL, NULL);
	ioctl(fd, 1);
	sleep(1);
	ioctl(fd, 0);
	sleep(1);
	ioctl(fd, 1);
	close(fd);
	return 0;
}
