#ifndef _TILE_H
#define _TILE_H

#include "affine.h"
#include "common.h"
#include "display.h"
#include "dma.h"
#include "filesystem.h"

template<unsigned length>
inline void loadTilesFromArray(Tile* destTiles, const unsigned short (&array)[length], unsigned short tileNum = 0) {
	dmaCopy(array, (destTiles + tileNum), length, halfword);
};

template<unsigned length>
inline void loadTilesFromArray(Tile* destTiles, const unsigned (&array)[length], unsigned short tileNum = 0) {
	dmaCopy(array, (destTiles + tileNum), length, word);
};

inline void loadTilesFromFile(File file, uint baseBlock, uint tileNum = 0) {
	dmaCopy(file.ptr, (Tile*)(tilesets + baseBlock) + tileNum + 1, file.length >> 2, word);
};

template<unsigned length>
inline void loadMapFromArray(ScreenBaseBlock& dest, const unsigned short (&array)[length]) {
	dmaCopy(array, &dest, length, halfword);
};

template<unsigned length>
inline void loadMapFromArray(ScreenBaseBlock& dest, const unsigned (&array)[length]) {
	dmaCopy(array, &dest, length, word);
};

/*******************************
**High-level wrapper interface**
*******************************/

struct MapHeader {
	uint width, height, numLayers;
} PACKED ALIGN_WORD;

/**
A simple tilemap

This tilemap uses only one screen base block to display its data; it makes use of the
map wrapping feature to simulate an arbitrarily large map.
*/
class Tilemap {
	public:
	Tilemap(void* dataLocation, uint charBase, uint screenBase) {
		
		width     = ((MapHeader*)dataLocation)->width;
		height    = ((MapHeader*)dataLocation)->height;
		numLayers = ((MapHeader*)dataLocation)->numLayers;
		layerSize = width * height;
		
		//Set up the backgrounds.
		for(uint i = 0; i < numLayers; ++i) {
			enableBackground(i);
			bgControls[i].charBaseBlock = charBase;
			bgControls[i].screenBaseBlock = screenBase + i;
			bgControls[i].paletteMode = singlePalette;
			bgControls[i].priority = 3 - i;
		}
		
		map = (ushort*)(dataLocation + sizeof(MapHeader));
		hwMap = tilemaps + screenBase;
		
		waitVBlank();
		for(uint layer = 0; layer < numLayers; ++layer) {
			for(uint x = 0; x < 32; ++x) {
				for(uint y = 0; y < 32; ++y) {
					hwMap[layer][y][x].data = getTile(x, y, layer);
				}
			}
		}
		
	}
	
	ushort getTile(uint x, uint y, uint layer) {
		return map[layer * layerSize + y * width + x];
	}
	
	void scrollBy(int dx, int dy) {
		x += dx;
		y += dy;
		
		const uint tx = x >> 3, ty = y >> 3;
		
		if(tx < loadedX) {
			for(uint layer = 0; layer < numLayers; ++layer) {
				for(uint i = ty; i < min(ty + 32, height); ++i) {
					hwMap[layer][i % 32][tx % 32].data = getTile(tx, i, layer);
				}
			}
		} else if(tx > loadedX && tx < width - 32) {
			const uint tx2 = tx + 31;
			for(uint layer = 0; layer < numLayers; ++layer) {
				for(uint i = ty; i < min(ty + 32, height); ++i) {
					hwMap[layer][i % 32][tx2 % 32].data = getTile(tx2, i, layer);
				}
			}
		}
		
		if(ty < loadedY) {
			for(uint layer = 0; layer < numLayers; ++layer) {
				for(uint i = tx; i < min(tx + 32, width); ++i) {
					hwMap[layer][ty % 32][i % 32].data = getTile(i, ty, layer);
				}
			}
		} else if(ty > loadedY && ty < height - 32) {
			const uint ty2 = ty + 31;
			for(uint layer = 0; layer < numLayers; ++layer) {
				for(uint i = tx; i < min(tx + 32, width); ++i) {
					hwMap[layer][ty2 % 32][i % 32].data = getTile(i, ty2, layer);
				}
			}
		}
		
		loadedX = tx;
		loadedY = ty;
		for(uint i = 0; i < numLayers; ++i) {
			bgPos[i].x = x % (32 * 8);
			bgPos[i].y = y % (32 * 8);
		}
	}
	
	uint scrollX() {
		return x;
	}
	
	uint scrollY() {
		return y;
	}
	
	private:
	uint width, height, numLayers, layerSize;
	ushort* map;
	ScreenBaseBlock* hwMap;
	uint x, y;
	uint loadedX, loadedY;
};

class AffineTilemap {
	public:
	AffineTilemap(void* dataLocation, uint charBase, uint screenBase) {
		
		size = ((MapHeader*)dataLocation)->width / 32;
		numLayers = ((MapHeader*)dataLocation)->numLayers;
		layerSize = size * size * (32 * 32);
		
		setBGIdentity(affine);
		
		//Set up the backgrounds.
		for(uint i = 2; i < numLayers + 2; ++i) {
			enableBackground(i);
			bgControls[i].charBaseBlock = charBase;
			bgControls[i].screenBaseBlock = screenBase + i - 2;
			bgControls[i].paletteMode = singlePalette;
			bgControls[i].size = size;
			bgControls[i].priority = 3 - i;
			bgAffines[i - 2] = affine;
		}
		
		//hwMap = tilemaps + screenBase;
		
		waitVBlank();
		dmaCopy(dataLocation + sizeof(MapHeader), tilemaps + screenBase, (layerSize * numLayers) / 4, word);
	}
	
	Fixed<8> scrollX() {
		return Fixed<8>(affine.dx);
	}
	
	Fixed<8> scrollY() {
		return Fixed<8>(affine.dy);
	}
	
	private:
	BGAffine affine;
	uint size;
	uint numLayers;
	uint layerSize;
};

#endif
