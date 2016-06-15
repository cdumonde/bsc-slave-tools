#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <curses.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "bsc-slave.h"
#include "rPodI2C.h"

#define SLV_ADDR      0x33
#define TX_BUF_SIZE   5

#define DEFAULT_DEVICE "/dev/i2c-slave"

int main(int argc, char **argv)
{
	char tx_buffer[TX_BUF_SIZE];
	int fd;
	uint8_t data;
	int length;

	int opt;
	int mode = 0;
	FILE *usage_file = stderr;
	int slave_addr = SLV_ADDR;
	const char *input = DEFAULT_DEVICE;

	while ((opt = getopt(argc, argv, "hxca:")) != -1) {
		switch (opt) {
		case 'd':
			mode = 0; // TODO defines
			break;
		case 'x':
			mode = 1; // TODO defines
			break;
		case 'c':
			mode = 2; // TODO defines
			break;
		case 'a':
			slave_addr = atoi(optarg);	
			break;
		case 'h':
			usage_file = stdout;
		default: /* '?' */
			fprintf(usage_file, "Usage: %s [-a addr] [-x|-c|-d] [device]\n", argv[0]);
			fprintf(usage_file, "defaults device: %s addr: %x\n", DEFAULT_DEVICE, SLV_ADDR);
			exit(usage_file == stdout ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}

	if (optind < argc) {
		input = argv[optind];
	}

	if ((fd = open(input, O_RDWR)) == -1) {
		perror("open i2c device");
		exit(EXIT_FAILURE);
	}

	if ((ioctl(fd, I2C_SLAVE, slave_addr) < 0)) {
		perror("setting i2c address");
		exit(EXIT_FAILURE);
	}

	initReceiver();

	while (1) {
		switch ((length = read(fd, tx_buffer, TX_BUF_SIZE))) {
			case -1:
				perror("read from i2c device");
				close(fd); // FIXME Probably useless should be left to the OS
				exit(EXIT_FAILURE);
			case 0:
				break;
		}

		for (int i = 0; i < length; i++){
			data = tx_buffer[i];
			receiveBytes(&data, 1); // I do not understand this one
			switch (mode) {
			case 1:
				printf("%c", data);
				break;
			case 2:
				printf("%02x ", data);
				break;
			default:
				printf("%d ", data);
				break;
			}
		}
	}

	close(fd);
	return 0;
}
