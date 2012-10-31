#ifndef _FILESYS_H
#define _FILESYS_H

#include <string.h>

#include "common.h"
#include "text.h"

/********************************************
**Partially based on GBFS by Damian Yerrick**
********************************************/

void* const romStart = (void*)0x8000000;
void* const romEnd = (void*)0xA000000;

extern const char* magicNumber;

struct FilesystemHeader {
	char magicNumber[24]; //Always equal to "GBA Filesystem Data\n\n\n\n\0"
	uint numDirs;
	
	private:
	uint padding;
};

struct DirEntry {
	char name[24];
	uint offset;
	uint numFiles;
};

struct FileEntry {
	char name[24];
	uint offset;
	uint length;
};

struct File {
	void* ptr;
	uint length;
};

class Filesystem {
	public:
	Filesystem(void* start = (void*)((uint)magicNumber + 64 - (uint)magicNumber % 32)) {
		//Find the header
		const char* ptr = (const char*)start;
		while(ptr < romEnd) {
			if(strcmp(ptr, magicNumber) == 0) {
				header = (FilesystemHeader*)ptr;
				dirs = (DirEntry*)(ptr + 32);
				return;
			}
			ptr += 32;
		}
		//If we get here, the file wasn't found
		header = 0;//(FilesystemHeader*)ptr;
		dirs = 0;
	}
	
	File getFile(const char* dirname, const char* filename) {
		File f;
		if(dirs == 0) {
			f.ptr = 0;
			return f;
		}
		for(uint i = 0; i < header->numDirs; ++i) {
			DirEntry* dir = dirs + i;
			if(strcmp(dir->name, dirname) == 0) {
				FileEntry* files = (FileEntry*)((uint)header + dir->offset);
				for(uint j = 0; j < dir->numFiles; ++j) {
					FileEntry* file = files + j;
					if(strcmp(file->name, filename) == 0) {
						f.ptr = (void*)((uint)header + file->offset);
						f.length = file->length;
						return f;
					}
				}
			}
		}
		//The file wasn't found
		fatalError(strcat("Unable to find file ", filename));
		return f;
	}
	
	FilesystemHeader* header;
	DirEntry* dirs;
};

#endif