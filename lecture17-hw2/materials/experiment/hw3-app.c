#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEV_FILE	"/dev/hw3"

int main(int argc, char *argv[])
{
	int fd;
	int ret = EXIT_FAILURE;
	char buf[10];

	fd = open(DEV_FILE, O_RDWR | O_NONBLOCK);
	if (fd == -1) {
		perror("Failed to open dev file");
		return EXIT_FAILURE;
	}

	ret = read(fd, buf, sizeof(buf));
	if (ret == -1)
		perror("read() error");
	else {
//		putchar(buf);
		printf("%s\n", buf);
	}

	close(fd);
	return EXIT_SUCCESS;
}

