#ifndef _TIMER_H
#define _TIMER_H

#include "common.h"

struct Timer {
	ushort value;
	
	uint frequency: 2;
	bool cascade: 1;
	
	private:
	uint padding: 3;
	
	public:
	bool raiseInterrupt: 1;
	bool enabled: 1;
};

Timer* const timers = (Timer*)0x4000100;

#endif