#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include "./module/eink_ioctl.h"

#define DISP_WHITE 0
#define DISP_BLACK 1

#define DtoR(X) ((X) * M_PI/(double)180.00)

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

// void writeSection(int offX, int offY, int sizeX, int sizeY)
//pos-pos-X-Y-section
void writeSection(int posX, int posY, int width, int height, bool color, uint8_t *section)
{
    int index = posX + width * posY;
    section[index] = color;
}

int main(void)
{
    int fd = open("/dev/einkChar", O_WRONLY);
    char test[] = "Write w/ IOCTL\0";

    struct pixelDataIn data;
    data.x = 50;
    data.y = 100;
    data.x1 = 10;
    data.y1 = 60;
    data.stringIn = test;
    data.stringLength = strlen(test);
    data.partLUT = false;

    // ioctl(fd, EINKCHAR_IOCWRXYLINE, &data);

    data.partLUT = true;
    ioctl(fd, EINKCHAR_IOCWRLUT, &data);

    ioctl(fd, EINKCHAR_IOCWRCHAR, &data);

    uint8_t *sec = malloc(20 * 20);

    // for(int i = 0; i < 20; i ++)
    // {
    //     for(int j = 0; j < 20; j++)
    //     {
    //         writeSection(i, j, 20, 20, DISP_WHITE, sec);
    //     }
    // }

    // for(int i = 0; i < 20; i ++)
    // {
    //     writeSection(i, i, 20, 20, DISP_BLACK, sec);
    // }

    // data.x = 0;
    // data.y = 0;
    // data.x1 = 20;
    // data.y1 = 20;
    // data.sectionData = sec;
    // ioctl(fd, EINKCHAR_IOCWRSECTION, &data);

    int x = round(40 * cos(DtoR(0)));
    int y = round(40 * sin(DtoR(0)));
    data.x = 20;
    data.y = 20;
    data.x1 = x + 20;
    data.y1 = y + 20;
    ioctl(fd, EINKCHAR_IOCWRXYLINE, &data);

    x = round(40 * cos(DtoR(45)));
    y = round(40 * sin(DtoR(45)));
    data.x = 40;
    data.y = 40;
    data.x1 = x + 40;
    data.y1 = y + 40;
    ioctl(fd, EINKCHAR_IOCWRXYLINE, &data);

    x = round(40 * cos(DtoR(90)));
    y = round(40 * sin(DtoR(90)));
    data.x1 = 80;
    data.y1 = 80;
    data.x = x + 80;
    data.y = y + 80;
    ioctl(fd, EINKCHAR_IOCWRXYLINE, &data);

    free(sec);

    close(fd);
    return 0;
}