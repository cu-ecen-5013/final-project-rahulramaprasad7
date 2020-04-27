ifeq ($(CC),)
   	CC = $(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
   	CFLAGS = -g -Wall -Werror -I.
endif

ifeq ($(LDFLAGS),)
   	LDFLAGS = -lrt -pthread -lm
endif

# first target all- default target. Calls native gcc compiler if Cross_COMPILE not described

all:
	$(CC) $(CFLAGS) client.c MadgwickAHRS.c -o client $(LDFLAGS)

clean:
	rm -f client
	
