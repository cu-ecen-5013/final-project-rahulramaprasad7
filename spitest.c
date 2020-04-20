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
#include "Lucida_Console_8pts.h"
#include "lcd_driver.h"

#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])

static const char *device = "/dev/spidev0.0";

int fd;

int main(void)
{
    fd = open(device, O_RDWR);

    if (wiringPiSetupGpio () == -1) {
        printf("Wiring pi fault\n");
        return 1 ;
    }

    pinMode (0, OUTPUT) ;         // aka BCM_GPIO pin 17
    pinMode (2, OUTPUT) ;         // aka BCM_GPIO pin 27

    pinMode (12, OUTPUT) ;
    pinMode (13, OUTPUT) ;
    pinMode (14, OUTPUT) ;

    pinMode(25, OUTPUT);

    digitalWrite(25, LOW);
    delay(100);
    digitalWrite(25, HIGH);

    lcdInit();
    // uint8_t t = 0x12;
    // write(fd, &t, 1);

    //(320x240 resolution)
    //    selectCS(0);
    drawLineX(20, 300, 220, GREEN);
    drawLineY(0, 220, 20, GREEN);

    //LCD UI
    //X-axis labels
    writeString(20,480 , WHITE, "0");
    writeString(84, 480, WHITE, "Fs/4");
    writeString(148, 480, WHITE, "Fs/2");
    writeString(212, 480, WHITE, "3Fs/4");
    writeString(276, 480, WHITE, "Fs");

    //Y-axis labels
    writeString(0, 460, WHITE, "20");
    writeString(0, 440, WHITE, "40");
    writeString(0, 420, WHITE, "60");
    writeString(0, 400, WHITE, "80");
    writeString(0, 380, WHITE, "100");
    writeString(0, 360, WHITE, "120");
    writeString(0, 340, WHITE, "140");
    writeString(0, 320, WHITE, "160");
    writeString(0, 300, WHITE, "180");
    writeString(0, 280, WHITE, "200");
    writeString(0, 260, WHITE, "220");

    close(fd);
}