#include <zmcu/dev/atmega328p.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "ssd1306.h"
#include "zfont.h"

void
print(char *s)
{
	uart_write((uint8_t *)s, strlen(s));
}

zfont_t *ascii_8;

int
main(void)
{
	char const *text = "test";

	gpio_init();
	timer0_init(CSx64, WGM0_NORMAL_0xFF);
	uart_init();
	i2c_init();
	sei();

	gpio_led(1);

	print("scanning I2C bus\r\n");
	for (uint8_t i = 1; i < 128; i++) {
		if (i2c_write(i, NULL, 1) == 0)
			print("answer\r\n");;
	}
	print("scanning done\r\n");

	if (ssd1306_init() == -1)
		print("could not init screen\r\n");
	ssd1306_point(4, 4);
	zfont_draw_text((zpoint_t){ 0, 0 }, 30, &text, ascii_8);
	ssd1306_flush();

	for (;;) {
		gpio_led(1);
		timer_sleep(1000000);

		gpio_led(0);
		timer_sleep(1000000);
	}

	return 0;
}

/* wrappers: plug libraries with each others */

int ssd1306_i2c_write(uint8_t addr, uint8_t const *buf, size_t sz)
{ return i2c_write(addr, buf, sz); }

void zfont_draw_point(uint16_t x, uint16_t y)
{ ssd1306_point(x, y); }

uint8_t zfont_read_rom(uint8_t const *addr)
{ return pgm_read_byte(addr); }
