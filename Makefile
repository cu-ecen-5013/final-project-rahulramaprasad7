all: lcd

C_FILE := \
	spitest.c \
	Lucida_Console_8pts_narrow.c \
	Lucida_Console_8pts.c \
	lcdDriver.c

lcd: ${C_FILE}
	gcc ${C_FILE} -lwiringPi -o spitest

# lcdExe: lcd


clean:
	rm spitest