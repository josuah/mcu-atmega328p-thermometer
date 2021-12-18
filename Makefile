LDFLAGS = -Wl,-Map=${BIN}.$map
CFLAGS = -g -Wall -Wextra -std=c99 -pedantic
CPPFLAGS = -Ilibzfont/include
PORT = /dev/ttyU0
BIN = main
OBJ = zfont.$o ssd1306.$o

ZMCU = ../libzmcu
include ${ZMCU}/lib/libzmcu.atmega328p.mk

all: ${BIN}.$elf ${BIN}.$asm
${BIN}.$elf: ${OBJ}
clean: clean.zmcu

libzfont:
	git clone git://git.josuah.net/libzfont
