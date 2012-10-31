#ifndef _COMMON_H
#define _COMMON_H

#define ALIGN_WORD __attribute__((aligned(4)))
#define ALIGN_HALFWORD __attribute__((aligned(2)))
#define PACKED __attribute__((packed))

#define EWRAM_DATA __attribute__((section(".ewram")))
#define IWRAM_DATA __attribute__((section(".iwram")))
#define  EWRAM_BSS __attribute__((section(".sbss")))

#define EWRAM_CODE __attribute__((section(".ewram"), long_call))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))



typedef unsigned short ushort;
typedef unsigned int uint;
typedef char byte;
typedef unsigned char ubyte;

inline uint min(uint a, uint b) {
	return (a < b)? a: b;
};

#endif