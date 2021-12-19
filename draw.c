#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <ctype.h>
#include "draw.h"

#define DIST(x, y) ((x) > (y) ? (x) - (y) : (y) - (x))
#define DIREC(x, y) ((x) < (y) ? 1 : (x) > (y) ? -1 : 0)

typedef struct {
	uint8_t width, height;
	font_t *bitmap;
} glyph_t;

static glyph_t
font_get_glyph(font_t *f, char c)
{
	uint8_t const *p = f;
	glyph_t g;

	assert(c >= ' ' || c <= '~');

	g.height = font_get_byte(p++);
	for (char i = ' '; i <= c; i++) {
		g.width = font_get_byte(p++);
		g.bitmap = p;
		p += (g.width * g.height + 7) / 8;
	}
	return g;
}

static void
draw_glyph(point_t p, glyph_t g)
{
	point_t p0 = { 0, 0 };

	for (p0.x = 0; p0.x < g.width; p0.x++) {
		for (p0.y = 0; p0.y < g.height; p0.y++) {
			uint8_t byte;
			size_t i;

			i = p0.y * g.width + p0.x;
			byte = font_get_byte(g.bitmap + i / 8);
			if (byte & 1<<i%8)
				draw_point(p.x + p0.x, p.y + p0.y);
		}
	}
}

point_t 
draw_text(point_t p, uint16_t x, char const **sp, font_t *f)
{
	char const *s;

	for (s = *sp; *s != '\0'; s++) {
		glyph_t g;

		g = font_get_glyph(f, isprint(*s) ? *s : '?');
		if (p.x + g.width > x)
			break;
		draw_glyph(p, g);
		p.x += g.width + 1;
	}
	*sp = s;
	return p;
}

point_t
draw_paragraph(point_t p, uint16_t x0, uint16_t x1, char const **sp, font_t *f)
{
	char const *s;

	for (s = *sp;;) {
		p = draw_text(p, x1, &s, f);
		for (; *s == ' '; s++);
		if (*s == '\0')
			break;
		p.x = x0;
		p.y += font_get_byte(f+0);
	}
	*sp = s;
	return p;
}

void
draw_line(point_t p0, point_t p1)
{
	int16_t debt, d, dx, dy, sx, sy;

	sx = DIREC(p0.x, p1.x);
	sy = DIREC(p0.y, p1.y);
	dx = DIST(p0.x, p1.x);
	dy = DIST(p0.y, p1.y);
	debt = ((dx > dy) ? dx : -dy) / 2;
	while (p0.x != p1.x || p0.y != p1.y) {
		d = debt;
		if (d > -dx) {
			p0.x += sx;
			debt -= dy;  /* debt toward y after we wrote toward x */
		}
		if (d < dy) {
			p0.y += sy;
			debt += dx;  /* debt toward x after we wrote toward y */
		}
		draw_point(p0.x, p0.y);
	}
}
