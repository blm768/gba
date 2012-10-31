#ifndef _AFFINE_H
#define _AFFINE_H

#include "display.h"
#include "lookup.h"
#include "text.h"
#include "sprite.h"

template<uint frac>
struct Fixed {
	explicit Fixed(int v = 0): value(v) {};
	
	Fixed(int wholeValue, uint fracValue) {
		value = (wholeValue << frac) + fracValue; 
	}
	
	Fixed operator+(Fixed other) volatile {
		return Fixed(value + other.value);
	}
	
	Fixed& operator+=(Fixed other) {
		value += other.value;
		return *this;
	}
	
	volatile Fixed& operator+=(Fixed other) volatile {
		value += other.value;
		return *this;
	}
	 
	Fixed operator-(Fixed other) volatile {
		return Fixed(value - other.value);
	}
	
	Fixed& operator-=(Fixed other) {
		value -= other.value;
		return *this;
	}
	
	volatile Fixed& operator-=(Fixed other) volatile {
		value -= other.value;
		return *this;
	}
	
	template<uint otherFrac>
	Fixed operator* (Fixed<otherFrac> other) volatile {
		return Fixed((value * other.value) >> otherFrac);
	}
	
	template<uint otherFrac>
	Fixed operator/ (Fixed<otherFrac> other) volatile {
		return Fixed((value << otherFrac) / other.value);
	}
	
	Fixed operator-() volatile {
		return Fixed(-value);
	}
	
	bool operator==(Fixed other) volatile {
		return value == other.value;
	}
	
	bool operator<(Fixed other) volatile {
		return value < other.value;
	}
	
	bool operator<=(Fixed other) volatile {
		return value <= other.value;
	}
	
	bool operator>(Fixed other) volatile {
		return value > other.value;
	}
	
	bool operator>=(Fixed other) volatile {
		return value >= other.value;
	}
	
	Fixed& operator=(Fixed other) {
		value = other.value;
		return *this;
	}
	
	volatile Fixed& operator=(Fixed other) volatile {
		value = other.value;
		return *this;
	}
	
	int value;
} PACKED ALIGN_WORD;

inline Fixed<12> gbaSin(Fixed<12> angle) {
	return Fixed<12>(sinLookup[((angle * Fixed<8>(radToLookup)).value >> 12) & 0b111111111]);
}

inline Fixed<12> gbaCos(Fixed<12> angle) {
	return gbaSin(angle + Fixed<12>(piFixed12 >> 1));
}

inline void setSpriteAffine(Fixed<12> angle, Fixed<8> invScaleX, Fixed<8> invScaleY, SpriteAffine& affine) {
	Fixed<12> ca = gbaCos(angle), sa = gbaSin(angle);
	affine.pa = ( ca.value * invScaleX.value) >> 12;
	affine.pb = (-sa.value * invScaleX.value) >> 12;
	affine.pc = ( sa.value * invScaleY.value) >> 12;
	affine.pa = ( ca.value * invScaleY.value) >> 12;
}

inline void setSpriteIdentity(SpriteAffine& affine) {
	affine.pa = 1 << 8;
	affine.pb = 0;
	affine.pc = 0;
	affine.pd = 1 << 8;
}

inline void setBGAffine(Fixed<12> angle, Fixed<8> dx, Fixed<8> dy, Fixed<8> invScaleX, Fixed<8> invScaleY,
						BGAffine& affine) {
	affine.dx = dx.value;
	affine.dy = dy.value;

	Fixed<12> ca = gbaCos(angle), sa = gbaSin(angle);
	int pa = ( ca.value * invScaleX.value) >> 12;
	int pb = (-sa.value * invScaleX.value) >> 12;
	int pc = ( sa.value * invScaleY.value) >> 12;
	int pd = ( ca.value * invScaleY.value) >> 12;
	
	affine.pa = pa;
	affine.pb = pb;
	affine.pc = pc;
	affine.pd = pd;
}

inline void setBGAffine(Fixed<12> angle, int dx, int dy, Fixed<8> invScaleX, Fixed<8> invScaleY,
						Fixed<8> pivotX, Fixed<8> pivotY, BGAffine& affine) {

	Fixed<12> ca = gbaCos(angle), sa = gbaSin(angle);
	int pa = ( ca.value * invScaleX.value) >> 12;
	int pb = (-sa.value * invScaleX.value) >> 12;
	int pc = ( sa.value * invScaleY.value) >> 12;
	int pd = ( ca.value * invScaleY.value) >> 12;
	
	affine.pa = pa;
	affine.pb = pb;
	affine.pc = pc;
	affine.pd = pd;
	
	affine.dx = pivotX.value - (pa * dx + pb * dy);
	affine.dy = pivotY.value - (pc * dx + pd * dy);
}

inline void setBGIdentity(BGAffine& affine) {
	affine.dx = 0;
	affine.dy = 0;
	affine.pa = 1 << 8;
	affine.pb = 0;
	affine.pc = 0;
	affine.pd = 1 << 8;
}

#endif