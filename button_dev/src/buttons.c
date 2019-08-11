#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd = open("/dev/butDev", O_RDWR);
//	write(fd, NULL, NULL);
	while(1){;};
	return 0;
}
