#include <zmcu/dev/atmega328p.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "ssd1306.h"
#include "draw.h"
#include "util.h"
#include "font.h"

int
main(void)
{
	char const *text = "Yataaaa !";

	gpio_init();
	timer0_init(CSx64, WGM0_NORMAL_0xFF);
	uart_init();
	i2c_init();
	sei();

	gpio_led(1);

	if (ssd1306_init() == -1)
		put("could not init screen\r\n");
	draw_point(4, 4);
	draw_text((point_t){ 0, 0 }, 100, &text, progmem_ascii_13);
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

int
ssd1306_i2c_write(uint8_t addr, uint8_t const *buf, size_t sz)
{
	return i2c_write(addr, buf, sz);
}

void
draw_point(uint16_t x, uint16_t y)
{
	ssd1306_point(x, y);
}

uint8_t
font_get_byte(uint8_t const *addr)
{
	return pgm_read_byte((uintptr_t)addr);
}

int
print(char const *s)
{
	return uart_write((uint8_t *)s, strlen(s));
}
