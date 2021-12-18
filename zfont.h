#ifndef ZFONT_H
#define ZFONT_H

typedef struct { uint16_t x, y; } zpoint_t;
typedef uint8_t const zfont_t;

/* to be set to a function that reads one byte of the font at `ptr` position
 * font from the ROM */
extern uint8_t zfont_read_rom(uint8_t const *ptr);

/* be set to a function that prints one pixel at coordinates (`x`, `y`)
 * starting from top left corner */
extern void zfont_draw_point(uint16_t x, uint16_t y);

/* draw string pointed by `ps` with font `f` at position `p` until `x` then
 * return the new absolute position of p with only vertical position changed */
zpoint_t zfont_draw_text(zpoint_t, uint16_t max, char const **s, zfont_t *f);

/* draw string pointed by `sp` with font `f` starting at position `p` until
 * `x1`, coming back to the margin `x0` after a line wrap */
zpoint_t zfont_draw_paragraph(zpoint_t p, uint16_t x0, uint16_t x1, char const **sp, zfont_t *f);

#endif
