#ifndef _TEXT_H
#define _TEXT_H

#include <stdio.h>

#include "bios.h"
#include "display.h"
#include "font.h"
#include "sound.h"

/*IMPORTANT NOTES:
	* Currently only works in Mode 0
	* Uses BG palette color 1 as the text color
*/

class TextLayer {
	public:
	TextLayer(uint background, uint charBlock, uint screenBlock): 
		bgControl(bgControls[background]),
		tilemap((TileEntry*)(tilemaps + screenBlock)) {
			enableBackground(background);
			bgControl.charBaseBlock = charBlock;
			bgControl.screenBaseBlock = screenBlock;
			bgControl.paletteMode = multiPalette;
			bgControl.priority = 3 - background;
			
			BitUnpackParams params = {fontTilesLen, 1, 4};
			
			bitUnpack(fontTiles, tilesets + charBlock, &params);
		};
		
	void writeChar(char c, uint x, uint y) {
		tilemap[(y << 5) + x].data = c - 32;
	}
	
	void write(const char* str, uint x, uint y) {
		uint startX = x;
		uint i = 0;
		char c = str[0];
		while(c != '\0') {
			if(c == '\n') {
				x = startX;
				++y;
			} else {
				writeChar(c, x, y);
				++x;
			}
			++i;
			c = str[i];
		}
	}
	
	void write(int i, uint x, uint y) {
		char buf[50];
		sprintf(buf, "%i", i);
		write(buf, x, y);
	}
	
	void write(uint i, uint x, uint y) {
		char buf[50];
		sprintf(buf, "%u", i);
		write(buf, x, y);
	}
	
	void write(const void* ptr, uint x, uint y) {
		char buf[50];
		sprintf(buf, "%p", ptr);
		write(buf, x, y);
	}
	
	void loadFont(const void* data, uint length = 192) {
		BitUnpackParams params = {length, 1, 4, 0};
		bitUnpack(data, tilesets + bgControl.charBaseBlock, &params);
	}
	
	template<unsigned length>
	void loadFont(uint const (&array)[length]) {
		BitUnpackParams params = {length, 1, 4, 0};
		bitUnpack(array, tilesets + bgControl.charBaseBlock, &params);
	};
	
	void clear() {
		for(uint i = 0; i < 32 * 32; ++i) {
			tilemap[i].data = 0;
		}
	}
	
	private:
	BackgroundControl& bgControl;
	TileEntry* tilemap;
	
};

inline void fatalError(const char* msg) {
	sound.enabled = false;
	
	//Reset graphics:
	dispControl.mode = 0;
	dispControl.enableSprites = false;
	dispControl.forceBlank = false;
	dispControl.spriteMappingMode = spriteMap1D;
	
	TextLayer text(0, 0, 8);
	text.loadFont(fontTiles);
	
	bgPalette[0] = makeColor(0, 0, 31);
	bgPalette[1] = makeColor(31, 31, 31);
	text.write(msg, 0, 0);
	
	while(true) 
		{}
}

inline void fatalError(int i) {
	char buf[50];
	sprintf(buf, "%u", i);
	fatalError(buf);
}



#endif