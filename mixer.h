#ifndef _MIXER_H
#define _MIXER_H

#include "bios.h"

//Interface for the GBA BIOS mixer
SoundWorkArea workArea;

inline void initMixer() {
	//To do: zero out work area first?
	soundDriverInit(&workArea);
	isrTable[intVBlank] = (ISR)soundDriverVSync;
}

inline void playSound(WaveData* data) {
	//To do: change this so it doesn't use this function?
	initInterrupts();
	enableVBlankInterrupt();
	SoundChannel& channel = workArea.channels[0];
	channel.data = data;
	channel.frequency = data->frequency;
	channel.volLeft = 255;
	channel.volRight = 255;
	channel.status = channelPlaying;
}

#endif