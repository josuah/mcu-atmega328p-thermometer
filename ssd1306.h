#ifndef SSD1306_H
#define SSD1306_H

/* controler for small monochrome OLED screen */

/* screen height in pixels */
#ifndef SSD1306_ROWS
#define SSD1306_ROWS	32
#endif

/* screen width in pixels */
#ifndef SSD1306_COLS
#define SSD1306_COLS	128
#endif

/* can be changed hardware-side, often by soldering an extra pin to ground */
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR 0x3C
#endif

/* to set to a function that writes `buf` of size `sz` to an I2C bus at `addr` */
int ssd1306_i2c_write(uint8_t addr, uint8_t const *buf, size_t sz);

/* send control commands to the screen to make it ready to draw */
int ssd1306_init(void);

/* send the buffer to the actual screen backend */
int ssd1306_flush(void);

/* print a dot on the internal buffer at coordinamtes (x, y) starting
 * from top-left */
void ssd1306_point(uint16_t x, uint16_t y);

/* reset the software buffer to 0x00: clear the screen */
void ssd1306_clear(void);

#endif
