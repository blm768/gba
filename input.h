#ifndef _INPUT_H
#define _INPUT_H

struct KeyStatus {
	bool aUp: 1;
	bool bUp: 1;
	bool selectUp: 1;
	bool startUp: 1;
	bool rightUp: 1;
	bool leftUp: 1;
	bool upUp: 1;
	bool downUp: 1;
	bool rUp: 1;
	bool lUp: 1;
};

KeyStatus& keys = *(KeyStatus*)0x4000130;

//TODO: Keyboard interrupts

#endif