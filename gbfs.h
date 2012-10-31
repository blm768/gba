#ifndef _FILESYS_H
#define _FILESYS_H

#include "common.h"

/********************************************
**Partially based on GBFS by Damian Yerrick**
********************************************/

struct FilesystemHeader {
	char[21] magic; //Always equal to "GBA Filesystem Data\n\0"
	uint numDirs;
}

struct FilesytemDirEntry {
	char[24] name;
	uint numFiles;
}

struct FilesystemFileEntry {
	char[24] name;
	uint offset;
	uint length;
}

struct File {
	char[24] name;
	void* ptr;
	uint length;
}

class Filesystem {
	Filesystem() {
		//Find the header
		
	}
	
	
}

#endif