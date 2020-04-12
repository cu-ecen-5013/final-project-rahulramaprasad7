all: lcd

C_FILE := \
	spitest.c \
	Lucida_Console_8pts_narrow.c \
	Lucida_Console_8pts.c \
	lcdDriver.c

lcd: ${C_FILE}
	pwd
	# gcc ./spitest.c ./Lucida_Console_8pts.c ./Lucida_Console_8pts_narrow.c ./lcdDriver.c -lwiringPi -o spitest
	gcc ${C_FILE} -lwiringPi -o spitest

# lcdExe: lcd


clean:
	rm spitest