#include <zmcu/dev/atmega328p.h>
#include <string.h>

void
print(char *s)
{
	uart_write((uint8_t *)s, strlen(s));
}

#define cli() __asm__ __volatile__ ("cli" ::: "memory")
#define sei() __asm__ __volatile__ ("sei" ::: "memory")

int
main(void)
{
	gpio_init();
	timer_init(0, CSx64, WGM0_NORMAL_0xFF);
	uart_init();
	sei();

	print("let's-a-go\r\n");
	timer_sleep(1000000);
	for (;;) {
		timer_sleep(1000000);
		print("led on\r\n");
		gpio_led(1);
		timer_sleep(1000000);
		print("led off\r\n");
		gpio_led(0);
	}

	return 0;
}
