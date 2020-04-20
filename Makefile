C_FILES := \
			epd1in541.c \
			epdif1.c \
			main.c \
			Lucida_Console_8pts_narrow.c \
			Lucida_Console_8pts.c

EXE := epaper

all: out

out: ${C_FILES}
	gcc ${C_FILES} -lwiringPi -o ${EXE} 

clean:
	rm ${EXE}
