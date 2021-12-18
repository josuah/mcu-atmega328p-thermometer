#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "ssd1306.h"

/* i2c packets for filling the R/S pin with 0/1 */
#define	CTRL (0<<7 | 0<<6)
#define	DATA (0<<7 | 1<<6)

#define SET_MODE 0x20
#define MODE_HORIZ 0
#define MODE_VERT 1
#define MODE_PAGE 2

#define SET_PAGE_RANGE 0x22

#define SET_POS 0x40

#define SET_CONTRAST 0x81

#define SET_PUMP 0x8D
#define PUMP_ON 0x14

#define SET_ALL_RAM 0xA4
#define SET_ALL_ON 0xA5

#define SET_INV_OFF 0xA6
#define SET_INV_ON 0xA7

#define SET_DISP_OFF 0xAE
#define SET_DISP_ON 0xAF

#define SET_CLOCK 0xD5
#define CLOCK_DIVIDE 0<<0
#define CLOCK_FREQ 15<<4

#define SET_COM_PIN 0xDA
#define COM_PIN_ALT_NO_REMAP 0x02

#define SET_COM_0_TO_N 0xC0
#define SET_COM_N_TO_0 0xC8

#define SET_REMAP_0 0xA0
#define SET_REMAP_127 0xA1

#define SET_OFFSET 0xD3
#define SET_MULTIPLEX 0xA8

static uint8_t pixbuf[1 + SSD1306_ROWS/8 * SSD1306_COLS];

int
ssd1306_init(void)
{
	static const uint8_t buf[] = {
		CTRL,

		/* only first half of the screen */
		SET_PAGE_RANGE, 0, SSD1306_ROWS / 8 - 1,

		/* rule for selecting the next segment to write
		 * pixels to */
		SET_MODE, MODE_HORIZ,

		/* seems to be required or the screen stays balck
		 * maybe related to how the pins are connected to
		 * the PCB */
		SET_COM_PIN, COM_PIN_ALT_NO_REMAP,

		/* support 5V instead of 7/12V min */
		SET_PUMP, PUMP_ON,

		/* only works after pump configured */
		SET_DISP_ON,
	};

	ssd1306_clear();
	if (ssd1306_i2c_write(SSD1306_I2C_ADDR, buf, sizeof buf) == -1)
		return -1;
	return ssd1306_flush();
}

int
ssd1306_flush(void)
{
	return ssd1306_i2c_write(SSD1306_I2C_ADDR, pixbuf, sizeof pixbuf);
}

void
ssd1306_clear(void)
{
	memset(pixbuf, 0x00, sizeof pixbuf);
	pixbuf[0] = DATA;  /* rest of the buffer for data */
}

/*
 * scanning is configured by ssd1306_init() to go this way:
 *
 *	------ COLS ------		Seg: (ie) from 008 to 00F
 *	000 008 010 ... 3F0	|	Row: (ie) from 400 to 7FF
 *	 : / : / :  ...  : }Seg	|
 *	007 00F 017 ... 3FF	|
 *	 .---------------'	|
 *	400 408 410 ... 7F0	|ROWS
 *	 : / : / :  ...  :	|
 *	407 40F 417 ... 7FF	|
 *	 :   :   :   :   :	:
 */
void
ssd1306_point(uint16_t x, uint16_t y)
{
	uint8_t *seg;

	if (x >= SSD1306_COLS || y >= SSD1306_ROWS)
		return;
	seg = pixbuf + 1 + y/8 * SSD1306_COLS + x;
	assert(seg < pixbuf + sizeof pixbuf);
	*seg |= 1<<(y % 8);
}
