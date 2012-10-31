#include "sprite.h"

SpriteInitializer sprInit;

uint firstSpriteObject = 0;
uint freeSprites = 128;

void Sprite::update() {}

void Sprite::draw() {
	//Update OAM
	*obj = shadow;
}

Sprite::~Sprite() {
	if(isOwner)
		freeSpriteObject(objNum);
}