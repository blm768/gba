#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "common.h"
#include "display.h"

enum InterruptType {intVBlank, intHBlank, intVCount, intTimer0, intTimer1, intTimer2, intTimer3, intSerial,
					intDMA0, intDMA1, intDMA2, intDMA3, intKeyboard, intGamepak};

typedef void (*ISR)(void);

#define isrPtr *(ISR*)(0x3007FFC)

extern ISR isrTable[14];

#define intMaster (*(bool*)0x4000208)
#define intEnable (*(ushort*)0x4000200)

#define intAck (*(ushort*)0x4000202)
#define biosAck (*(ushort*)0x3007FF8)

IWRAM_CODE void isr();
IWRAM_CODE void gpInt();

inline void enableInterrupt(InterruptType type) {
	intMaster = false;
	intEnable |= 1 << type;
	intMaster = true;
}

//Enables interrupts and sets up a basic gamepak interrupt
inline void initInterrupts() {
	//Just to be safe
	intMaster = false;
	isrPtr = (ISR)isr;
	enableInterrupt(intGamepak);
	isrTable[intGamepak] = (ISR)gpInt;
	intMaster = true;
}

inline void enableVBlankInterrupt() {
	enableInterrupt(intVBlank);
	dispStatus.VBlankInt = true;
};


#endif
