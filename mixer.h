#ifndef _MIXER_H
#define _MIXER_H

#include "bios.h"
#include "sound.h"

struct Sound {
	void* data;
	size_t length;
};

class Mixer {
	this();
	
	enum {
		maxMusic = 4,
		maxSound = 2,
	}
	
	private:
	
}

inline void playSound() {
	
}

#endif