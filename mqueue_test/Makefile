C_FILES_TX :=	\
		tx.c

C_FILES_RX :=	\
		rx.c

RX_EXE :=	\
		rx

TX_EXE :=	\
		tx


ifeq ($(CC),)
	CC = $(CROSS_COMPILE)gcc
endif
ifeq ($(CFLAGS),)
	CFLAGS = -g -Wall -Werror
endif
ifeq ($(LDFLAGS),)
	LDFLAGS = -g -lrt -lpthread -lm
endif

all: _rx _tx

_rx: ${C_FILES_RX}
	$(CC) $? -o ${RX_EXE} $(LDFLAGS)

_tx: ${C_FILES_TX}
	$(CC) $? -o ${TX_EXE} $(LDFLAGS)

clean:
	rm ${RX_EXE} ${TX_EXE}