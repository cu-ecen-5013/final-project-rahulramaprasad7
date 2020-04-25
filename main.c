#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "./module/eink_ioctl.h"

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
    data.length = strlen(test);
    data.partLUT = false;


    ioctl(fd, EINKCHAR_IOCWRXYLINE, &data);

    data.partLUT = true;
    ioctl(fd, EINKCHAR_IOCWRLUT, &data);

    ioctl(fd, EINKCHAR_IOCWRCHAR, &data);

    close(fd);

    return 0;
}