#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "color.h"

/******************
**Display control**
******************/

enum SpriteMappingMode {spriteMap2D, spriteMap1D};

//Overlay bit-field struct for REG_DISPCNT
struct DisplayControl {
	unsigned mode: 3;
	bool isLegacy: 1;
	unsigned buffer: 1;
	bool allowOAM_HBLANK: 1;
	SpriteMappingMode spriteMappingMode: 1;
	bool forceBlank: 1;
	bool enableBG0: 1;
	bool enableBG1: 1;
	bool enableBG2: 1;
	bool enableBG3: 1;
	bool enableSprites: 1;
	bool enableWindows: 1;
};

struct DisplayStatus {
	volatile bool inVBlank: 1;
	volatile bool inHBlank: 1;
	volatile bool inVCount: 1;
	bool VBlankInt: 1;
	bool HBlankInt: 1;
	bool VCountInt: 1;
	
	private:
	unsigned padding: 2;
	
	public:
	unsigned vCountTrigger: 8;
};

struct MosaicControl {
	unsigned char backgroundHorizontal: 4;
	unsigned char backgroundVertical: 4;
	unsigned char spriteHorizontal: 4;
	unsigned char spriteVertical: 4;
};

enum BlendMode {none, blendAlpha, fadeWhite, fadeBlack};

enum BlendLayer {blendBG0 = 1, blendBG1 = 1 << 1, blendBG2 = 1 << 2, blendBG3 = 1 << 3, blendObj = 1 << 4, blendBackdrop = 1 << 5}; 

struct BlendControl {
	uint top: 6;
	BlendMode mode: 2;
	uint bottom: 6;
} PACKED ALIGN_HALFWORD;

struct BlendAlpha {
	uint top: 5;
	
	private:
	uint padding: 3;
	
	public:
	uint bottom: 5;
	
} PACKED ALIGN_HALFWORD;

/*********************
**Background control**
*********************/

struct BackgroundControl {
	unsigned priority: 2;
	unsigned charBaseBlock: 2;
	
	private:
	unsigned padding: 2;
	
	public:
	bool useMosaic: 1;
	PaletteMode paletteMode: 1;
	unsigned screenBaseBlock: 5;
	bool useAffineWrap: 1;
	unsigned size: 2;
} PACKED ALIGN_HALFWORD;

struct BackgroundPosition {
	short x, y;
} PACKED ALIGN_WORD;

struct BGAffine {
	short pa, pb, pc, pd;
	int dx, dy;
} ALIGN_WORD;

/********
**Tiles**
********/

struct MultiPaletteTile {
	unsigned data[8];
};

struct Tile {
	ubyte data[64];
};

struct TileEntry {
	ushort data;
	
	ushort tileNum() {
		return data & 0b11111111;
	}
	
	void tileNum(ushort value) {
		data &= ~0b11111111;
		data |= value;
	}
	
} PACKED ALIGN_HALFWORD;

const unsigned bgSizeChar32x32 = 0;
const unsigned bgSizeChar64x32 = 1;
const unsigned bgSizeChar32x64 = 2;
const unsigned bgSizeChar64x64 = 3;

const unsigned bgSizeAffine16 = 0;
const unsigned bgSizeAffine32 = 1;
const unsigned bgSizeAffine64 = 2;
const unsigned bgSizeAffine128 = 3;

typedef unsigned AffineScreenBaseBlock[512];

typedef TileEntry ScreenRow[32];
typedef ScreenRow ScreenBaseBlock[32];

typedef Tile CharBaseBlock[256];

/**************************
**Register/data locations**
**************************/

#define dispControl (*(DisplayControl*)0x4000000)
#define dispStatus (*(volatile DisplayStatus*)0x4000004)
#define vCount (*(volatile unsigned char*)0x4000006)

Color* const bitmapPixels = (Color*)0x6000000;

BackgroundControl* const bgControls = (BackgroundControl*)0x4000008;
BackgroundPosition* const bgPos = (BackgroundPosition*)0x4000010;
BGAffine* const bgAffines = (BGAffine*)0x4000020;

#define mosaic (*(MosaicControl*)0x400004C)

#define blend (*(BlendControl*)0x4000050)
#define alpha (*(BlendAlpha*)0x4000052)

Tile* const spriteset = (Tile*)0x6010000;
AffineScreenBaseBlock* const affineTilemaps = (AffineScreenBaseBlock*)0x6000000;
ScreenBaseBlock* const tilemaps = (ScreenBaseBlock*)0x6000000;
CharBaseBlock* const tilesets = (CharBaseBlock*)0x6000000;

/********************
**Utility functions**
********************/

inline void enableBackground(uint num) {
	if(num < 4)
		*(ushort*)&dispControl |= 1 << (8 + num);
};

#endif