#ifndef DRAW_H
#define DRAW_H

typedef struct { uint16_t x, y; } point_t;
typedef uint8_t const font_t;

/* to be set to a function that reads one byte of the font at `ptr` position
 * font from the ROM */
extern uint8_t font_get_byte(uint8_t const *ptr);

/* be set to a function that prints one pixel at coordinates (`x`, `y`)
 * starting from top left corner */
extern void draw_point(uint16_t x, uint16_t y);

/* draw string pointed by `ps` with font `f` at position `p` until `x` then
 * return the new absolute position of p with only vertical position changed */
point_t draw_text(point_t, uint16_t max, char const **s, font_t *f);

/* draw string pointed by `sp` with font `f` starting at position `p` until
 * `x1`, coming back to the margin `x0` after a line wrap */
point_t draw_paragraph(point_t p, uint16_t x0, uint16_t x1, char const **sp, font_t *f);

/* draw a line from point `p0` and `p1` */
void draw_line(point_t p0, point_t p1);

#endif
