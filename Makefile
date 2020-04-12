all: lcd

lcd: spitest.c
	gcc -o spitest spitest.c -lwiringPi

clean:
	rm spitest