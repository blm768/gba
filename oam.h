#ifndef _OAM_H
#define _OAM_H

#include "common.h"
#include "color.h"

//TODO: make sure that these enums are the size of a ushort?
enum SpriteDisplayMode {normal, affine, hidden, affineDouble};
enum SpriteBlendMode {spriteOpaque, spriteBlend, spriteWindow};
enum SpriteShape {square, wide, tall};

struct SpriteObject {
	//Attribute 0
	ushort att0;
	
	ushort y() {
		ushort y = att0 & 0b11111111;
		//If the value was negative, fill in the rest of the bits to make the ushort negative:
		if(y & (0b1 << 7))
			y |= ~0b11111111;
		return y;
	};
	
	void y(ushort value) {
		att0 &= ~0b11111111;
		att0 |= (value & 0b11111111);
	};

	SpriteDisplayMode displayMode() {
		return (SpriteDisplayMode)((att0 & (0b11 << 8)) >> 8);
	};
	
	void displayMode(SpriteDisplayMode value) {
		att0 &= ~(0b11 << 8);
		att0 |= (ushort)value << 8;
	};
	
	SpriteBlendMode blendMode() {
		return (SpriteBlendMode)((att0 & (0b11 << 10)) >> 10);
	};
	
	void blendMode(SpriteBlendMode value) {
		att0 &= ~(0b11 << 10);
		att0 |= (ushort)value << 10;
	};
	
	bool useMosaic() {
		return (att0 & (0b1 << 12)) >> 12;
	};
	
	void useMosaic(bool value) {
		att0 &= ~(0b1 << 12);
		att0 |= value << 12;
	};
	
	PaletteMode paletteMode() {
		return (PaletteMode)((att0 & (0b1 << 13)) >> 13);
	};
	
	void paletteMode(PaletteMode value) {
		att0 &= ~(0b1 << 13);
		att0 |= (ushort)value << 13;
	};
	
	SpriteShape shape() {
		return (SpriteShape)((att0 & (0b11 << 14)) >> 14);
	};
	
	void shape(SpriteShape value) {
		att0 &= ~(0b11 << 14);
		att0 |= (ushort)value << 14;
	};
	
	//Attribute 1
	ushort att1;
	
	ushort x() {
		ushort x = att1 & 0b111111111;
		//If the value was negative, fill in the rest of the bits to make the ushort negative:
		if(x & (0b1 << 8))
			x |= ~0b111111111;
		return x;
	};
	
	void x(ushort value) {
		att1 &= ~0b111111111;
		att1 |=  value & 0b111111111;
	};
	
	ushort affine() {
		return (att1 & 0b11111 << 9) >> 9;
	};
	
	void affine(ushort value) {
		att1 &= ~(0b11111 << 9);
		att1 |= value << 9;
	};
	
	bool flipHorizontal() {
		return (att1 & (0b1 << 11)) >> 11;
	};
	
	void flipHorizontal(bool value) {
		att1 &= ~(0b1 << 11);
		att1 |= value << 11;
	};
	
	bool flipVertical() {
		return (att1 & (0b1 << 12)) >> 12;
	};
	
	void flipVertical(bool value) {
		att1 &= ~(0b1 << 12);
		att1 |= value << 12;
	};
	
	ushort size() {
		return (att1 & (0b11 << 14)) >> 14;
	};
	
	void size(ushort value) {
		att1 &= ~(0b11 << 14);
		att1 |= value << 14;
	};
	
	//Attribute 2
	ushort att2;
	
	ushort tile() {
		return (att2 & 0b1111111111);
	};
	
	void tile(ushort value) {
		att2 &= ~0b1111111111;
		att2 |= value;
	};
	
	ushort priority() {
		return (att2 & 0b11) >> 10;
	};
	
	void priority(ushort value) {
		att2 &= ~(0b11 << 10);
		att2 |= value << 10;
	};
	
	ushort palette() {
		return (att2 & 0b1111) >> 12;
	};
	
	void palette(ushort value) {
		att2 &= ~(0b1111 << 12);
		att2 |= value << 12;
	};
	
	//"Attribute 3"
	private:
	short rotationParams;
} ALIGN_WORD;

struct SpriteAffine {
	private: unsigned short padding0[3];
	public: short pa;
	private: unsigned short padding1[3];
	public: short pb;
	private: unsigned short padding2[3];
	public: short pc;
	private: unsigned short padding4[3];
	public: short pd;
} ALIGN_WORD;

SpriteObject* const spriteObjects = (SpriteObject*)0x7000000;
SpriteAffine* const spriteAffines = (SpriteAffine*)0x7000000;

#endif