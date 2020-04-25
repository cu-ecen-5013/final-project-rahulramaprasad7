/*
 * eink_ioctl.h
 */

#ifndef EINK_IOCTL_H
#define EINK_IOCTL_H

#ifdef __KERNEL__
#include <asm-generic/ioctl.h>
#include <linux/types.h>
#else
#include <sys/ioctl.h>
#include <stdint.h>
#endif

struct pixelDataIn {
    int x;
    int y;
    char* stringIn;

    int x1;
    int y1;
    int lineLength;
    unsigned long length;
};

// Pick an arbitrary unused value from https://github.com/torvalds/linux/blob/master/Documentation/ioctl/ioctl-number.rst
#define EINK_IOC_MAGIC 0x16

// Define a write command from the user point of view, use command number 1
#define EINKCHAR_IOCWRCHAR _IOR(EINK_IOC_MAGIC, 1, struct pixelDataIn)
#define EINKCHAR_IOCWRXYLINE _IOR(EINK_IOC_MAGIC, 2, struct pixelDataIn)
/**
 * The maximum number of commands supported, used for bounds checking
 */
#define EINKCHAR_IOC_MAXNR 2

#endif /* EINK_IOCTL_H */