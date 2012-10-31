#ifndef _SPRITE_H
#define _SPRITE_H

#include <string.h>

#include "common.h"
#include "filesystem.h"
#include "text.h"
#include "oam.h"

/**
High-level wrapper around OAM sprites
*/

inline void loadSpritesFromFile(File file, ushort tileNum = 0) {
	dmaCopy(file.ptr, (Tile*)(spriteset) + tileNum + 1, file.length >> 2, word);
};

//Dummy class to make sure initialization happens:
class SpriteInitializer {
	public:
	SpriteInitializer() {
		for(uint i = 0; i < 128; ++i) {
			spriteObjects[i].displayMode(hidden);
			spriteObjects[i].y(i + 1);
		}
	};
};

extern uint firstSpriteObject;
extern uint freeSprites;

inline uint getSpriteObject() {
	if(freeSprites == 0)
		fatalError("No free OAM sprite blocks");
	uint objNum = firstSpriteObject;
	firstSpriteObject = spriteObjects[objNum].y();
	--freeSprites;
	return objNum;
}

inline void freeSpriteObject(uint objNum) {
	SpriteObject* obj = spriteObjects + objNum;
	obj->displayMode(hidden);
	obj->y(firstSpriteObject);
	firstSpriteObject = objNum;
	++freeSprites;
}

class Sprite {
	public:
	Sprite(uint tile, uint size, SpriteShape shape, PaletteMode pMode = singlePalette) {
		objNum = getSpriteObject();
		obj = spriteObjects + objNum;
		isOwner = true;
		
		//Set shadow's fields.
		memset(&shadow, 0, sizeof(shadow));
		shadow.tile(tile);
		shadow.size(size);
		shadow.shape(shape);
		shadow.displayMode(normal);
		shadow.paletteMode(pMode);
		shadow.y(0);
	}
	
	void update();
	
	void draw();
	
	~Sprite();
	
	private:
	int x, y;
	uint objNum;
	SpriteObject* obj;
	
	//Shadows the data until draw() is called to update OAM
	SpriteObject shadow;
	
	private:
	bool isOwner;
};

#endif
