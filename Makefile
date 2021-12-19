LDFLAGS = -Wl,-Map=${BIN}.$map
CFLAGS = -g -Wall -Wextra -std=c99 -pedantic
PORT = /dev/ttyU0
BIN = main
OBJ = util.$o draw.$o ssd1306.$o font.$o

ZMCU = ../libzmcu
include ${ZMCU}/lib/libzmcu.atmega328p.mk

all: ${BIN}.$elf ${BIN}.$asm
${BIN}.$elf: ${OBJ}
clean: clean.zmcu
