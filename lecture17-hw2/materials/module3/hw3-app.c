#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw3.h"

#define DEV_FILE	"/dev/hw3"
#define TIMEOUT		10000		/* msec */
#define BUF_SIZE	4096
#define COUNT		20

int main(int argc, char *argv[])
{
	int fd;
	int ret = EXIT_FAILURE;
	int i;
	struct pollfd pfd;
	int ready;
	int led_on;
	char buf[BUF_SIZE];

	fd = open(DEV_FILE, O_RDWR | O_NONBLOCK);
	if (fd == -1) {
		perror("Failed to open dev file");
		return EXIT_FAILURE;
	}

	i = 0;
	if (ioctl(fd, HW3IOC_KERN_CONTROL, &i) == -1) {
		perror("Error occurred on ioctl");
		goto end;
	}

	ioctl(fd, HW3IOC_GETLED, &led_on);

	pfd.fd = fd;
	pfd.events = POLLIN;
	for (i = 0; i < COUNT; ++i) {
		printf("Waiting for button interrupt [%d/%d]...\n", i+1, COUNT);
		ready = poll(&pfd, 1, TIMEOUT);
		if (ready < 0) {
			perror("poll() error");
			goto end;
		} else if (ready == 0) {
			fprintf(stderr, "poll() timeout\n");
			goto end;
		}
		if (!(pfd.revents & POLLIN)) {
			fprintf(stderr, "poll() returned with no POLLIN\n");
			goto end;
		}

		ret = read(fd, buf, BUF_SIZE);
		if (ret == -1)
			perror("read() error");
		else
			printf("read: %s\n", buf);

		/* Handle "release button" event */
		if (buf[0] == '0') {
			led_on ^= 0x1;
			//ioctl(fd, HW3IOC_SETLED, &led_on);
			sprintf(buf, "%d", led_on);
			write(fd, buf, 2);
		}
	}

	i = 1;
	ioctl(fd, HW3IOC_KERN_CONTROL, &i);
	ret = EXIT_SUCCESS;

end:
	close(fd);
	return ret;
}

