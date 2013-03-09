#ifndef _DMA_H
#define _DMA_H

#include <string.h>

#include "common.h"

enum DMASourceAdjustment {srcIncrement, srcDecrement, srcFixed, srcReload};
enum DMADestinationAdjustment {destIncrement, destDecrement, destFixed};
enum DMAChunkSize {halfword, word};
enum DMATimingMode {now, onVBlank, onHBlank, onRefresh};

struct DMAParams {
	private:
	unsigned padding0: 5;
	
	public:
	DMADestinationAdjustment destAdjust: 2;
	DMASourceAdjustment srcAdjust: 2;
	bool repeat: 1;
	DMAChunkSize chunkSize: 1;
	
	private:
	unsigned padding1: 1;
	
	public:
	DMATimingMode timingMode: 2;
	bool raiseInterrupt: 1;
	bool enable: 1;
} PACKED ALIGN_HALFWORD;

struct DMAControl {
	const void* src;
	void* dest;
	//Warning: this is in CHUNKS, not bytes!
	unsigned short count;
	
	DMAParams params;
} PACKED ALIGN_HALFWORD;

DMAControl* const dmaChannels = (DMAControl*)0x40000B0;
#define dma3 (dmaChannels[3])

inline void dmaClearParams(unsigned channel) {
	memset(&dmaChannels[channel].params, 0, sizeof(DMAParams));
};

//To do: make more like memcpy
inline void dmaCopy(const void* src, void* dest, unsigned short count, DMAChunkSize chunkSize) {
	dmaClearParams(3);
	dma3.src = src;
	dma3.dest = dest;
	dma3.count = count;
	
	DMAParams params;
	memset(&params, 0, sizeof(params));
	params.chunkSize = chunkSize;
	params.enable = true;
	
	dma3.params = params;
};

//To do: move.
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

#endif