#ifndef _SOUND_H
#define _SOUND_H

#include "common.h"

#include "timer.h"

enum WaveVolume {volume50, volume100};

enum {
	TICKS_PER_FRAME = 280896,
};

struct MasterSoundControl {
	uint lVolume: 3;
	
	private:
	uint padding0: 1;
	
	public:
	uint rVolume: 3;
	
	private:
	uint padding1: 1;
	
	public:
	bool lSquare1: 1;
	bool lSquare2: 1;
	bool lWave: 1;
	bool lNoise: 1;
	
	bool rSquare1: 1;
	bool rSquare2: 1;
	bool rWave: 1;
	bool rNoise: 1;
	
	//Second halfword
	uint volume: 2;
	WaveVolume volumeA: 1;
	WaveVolume volumeB: 1;
	
	private:
	uint padding2: 4;
	
	public:
	bool enableARight: 1;
	bool enableALeft: 1;
	uint timerA: 1;
	bool dmaResetA: 1;
	bool enableBRight: 1;
	bool enableBLeft: 1;
	uint timerB: 1;
	bool dmaResetB: 1;
	
	//Third halfword
	bool square1Active: 1;
	bool square2Active: 1;
	bool waveActive: 1;
	bool noiseActive: 1;
	
	private:
	uint padding3: 3;
	
	public:
	bool enabled: 1;
	
	private:
	uint padding4: 8;
};

typedef byte WaveFIFO[4];

WaveFIFO* const waveFIFOs = (WaveFIFO*)0x40000A0;

#define sound (*(MasterSoundControl*)0x4000080)

inline void initSoundDMA(unsigned channel, const void* src, void* dest) {
	dmaClearParams(channel);
	dmaChannels[channel].src = src;
	dmaChannels[channel].dest = dest;
	
	DMAParams params;
	memset(&params, 0, sizeof(params));
	params.destAdjust = destFixed;
	params.repeat = true;
	params.chunkSize = word;
	params.timingMode = onRefresh;
	params.raiseInterrupt = true;
	params.enable = true;
	
	dmaChannels[channel].params = params;
};

inline void initChannel(size_t channel, void* buf, size_t clocksPerSample) {
	//The DMA channel and sound channel are made to match.
	//To consider: allow them to be set manually?
	initSoundDMA(channel + 1, buf, &(waveFIFOs[channel]));
	
	Timer t;
	t.value = -clocksPerSample;
	
	timers[channel] = t;
}

inline void startChannel(size_t channel) {
	timers[channel].enable = true;
}

#endif