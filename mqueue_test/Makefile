C_FILES_TX :=	\
		tx.c

C_FILES_RX :=	\
		rx.c

RX_EXE :=	\
		rx

TX_EXE :=	\
		tx


all: _rx _tx

_rx: ${C_FILES_RX}
	gcc $? -o ${RX_EXE} -lrt -lpthread

_tx: ${C_FILES_TX}
	gcc $? -o ${TX_EXE} -lrt -lpthread

clean:
	rm ${RX_EXE} ${TX_EXE}