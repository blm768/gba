#ifndef _COLOR_H
#define _COLOR_H

#include "common.h"
#include "dma.h"

enum PaletteMode {multiPalette, singlePalette};

struct Color {
	ushort data;
	
	ushort r() {
		return data & 0b11111;
	}
	
	void r(ushort value) {
		data &= ~0b11111;
		data |= value;
	}
	
	ushort g() {
		return (data & 0b1111100000) >> 5;
	}
	
	void g(ushort value) {
		data &= ~0b1111100000;
		data |= value << 5;
	}
	
	ushort b() {
		return (data & (0b11111 << 10)) >> 10;
	}
	
	void b(ushort value) {
		data &= ~(0b11111 << 10);
		data |= value << 10;
	}
} PACKED ALIGN_HALFWORD;

//Using this instead of a constructor because that adds a VPTR to struct Color:
inline Color makeColor(unsigned short r, unsigned short g, unsigned short b) {
	Color c;
	c.r(r);
	c.b(b);
	c.g(g);
	return c;
};

template<unsigned length>
inline void loadPaletteFromArray(Color* palette, const unsigned short (&array)[length]) {
	dmaCopy(array, palette, halfword, length);
};

Color* const bgPalette = (Color*)0x5000000;
Color* const spritePalette = (Color*)0x5000200;

#endif