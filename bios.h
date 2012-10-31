#ifndef _BIOS_H
#define _BIOS_H

#include "common.h"
#include "interrupt.h"

//Just the header; should be followed by data
struct WaveData {
	ushort type;
	ushort status;
	uint frequency;
	uint loopStart;
	uint samples;
} PACKED ALIGN_HALFWORD;

const uint channelCount = 8;

enum ChannelStatus {channelPaused = 0, channelPlaying = 0x80, channelKeyOff = 0x40};

struct SoundChannel {
	ubyte status;
	
	private:
	ubyte r1;
	
	public:
	ubyte volRight;
	ubyte volLeft;
	ubyte attack;
	ubyte decay;
	ubyte sustain;
	ubyte release;
	
	private:
	ubyte r2[4];
	
	public:
	uint frequency;
	WaveData* data;
	
	private:
	uint r3[6];
	ubyte r4[4];
} PACKED ALIGN_HALFWORD;

struct SoundWorkArea {
	uint ident;
	
	private:
	volatile ubyte dmaCount;
	
	public:
	ubyte reverb;
	
	private:
	ushort d1;
	void (*func)();
	int intp;
	void* internal;
	
	public:
	SoundChannel channels[channelCount];

	private:
	//To do: determine actual PCM buffer size
	byte pcmBuf[512];
	
} PACKED ALIGN_HALFWORD;

extern "C" {

	struct BitUnpackParams {
		ushort sourceLength;
		ubyte sourceWidth;
		ubyte destWidth;
		unsigned destOffset;
	};
	
	void vBlankIntrWait();
	
	void bitUnpack(const void* src, void* dest, BitUnpackParams* params);

	void decompressLZ77VRAM(const void* src, void* dest);
	
	//Sound
	
	void soundDriverInit(SoundWorkArea* workArea);
	void soundDriverMain();
	void soundDriverVSync();
	void soundDriverVSyncOff();
	void soundDriverVSyncOn();
}

inline void waitVBlank() {
	if(intMaster and intEnable & (1 << intVBlank)) {
		vBlankIntrWait();
	} else {
		//If we're in a VBlank, wait until it finishes.
		while(dispStatus.inVBlank) {};
		//Now wait for the start of the next VBlank.
		while(!dispStatus.inVBlank) {};
	}
};

#endif