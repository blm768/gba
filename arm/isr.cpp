#include "../common.h"
#include "../interrupt.h"

//Based on the TONC interrupt handler

ISR isrTable[14] = {0};

extern "C" uint callSysISR(ISR isr);

IWRAM_CODE void isr() {
	ISR foundIsr = 0;
	
	uint oldEnable = intEnable;
	uint raisedInterrupts = intEnable & intAck;
	
	//Acknowledge interrupts.
	intAck = raisedInterrupts;
	biosAck |= raisedInterrupts;
	
	//Find the interrupt in the table.
	for(uint i = 0; i < 14; ++i) {
		uint mask = 1 << i;
		if(raisedInterrupts & mask) {
			foundIsr = isrTable[i];
			break;
		}
	}
	
	if(!foundIsr)
		return;
	
	//Disable this interrupt and global interrupts.
	intMaster = false;
	intEnable &= ~raisedInterrupts;
	
	callSysISR(foundIsr);
	
	//Re-enable the interrupt.
	intMaster = false;
	intEnable = oldEnable;
	intMaster = true;
}

//Basic Gamepak interrupt
//To do: disable sound
IWRAM_CODE void gpInt() {
	while(true) {}
}
