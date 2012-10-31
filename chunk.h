#ifndef _CHUNK_H
#define _CHUNK_H

/*
Format:
	map width (4)
	map height (4)
	number of layers (4; includes padding for alignment)
	Array of offsets to chunk data from start of chunk data section (4 bytes each; indexed by [y][x])
	Chunk data (aligned on 4-byte boundaries); chunks at the same (x, y) location but different layers are stored contiguously.
*/

struct ChunkHeader {
	uint width;
	uint height;
	uint numLayers;
};

struct Chunk {
	ushort data[1024];
};

struct ChunkMap {
	ChunkMap(void* headerLocation) {
		header = (ChunkHeader*)headerLocation;
		width = header->width;
		height = header->height;
		numLayers = header->numLayers;
		chunkOffsets = (uint*)(header + sizeof(ChunkHeader));
		chunkStart = (void*)(chunkOffsets + width * height);
	}
	
	void* getChunk(uint x, uint y, uint layer) {
		return chunkStart + chunkOffsets[(y * width + x) * numLayers + layer];
	}
	
	ChunkHeader* header;
	uint* chunkOffsets;
	void* chunkStart;
	
	uint width;
	uint height;
	uint numLayers;
};

#endif