#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <wiringPi.h>

#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])

static const char *device = "/dev/spidev0.0";

int fd;

int main(void)
{
    fd = open(device, O_RDWR);

    if (wiringPiSetup () == -1) {
        printf("Wiring pi fault\n");
        return 1 ;
    }

    pinMode (0, OUTPUT) ;         // aka BCM_GPIO pin 17

    close(fd);
}