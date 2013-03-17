#ifndef _TIMER_H
#define _TIMER_H

#include "common.h"

struct Timer {
	ushort value;
	
	enum {
		max = (1 << 16) - 1,
	};
	
	uint frequency: 2;
	bool cascade: 1;
	
	private:
	uint padding: 3;
	
	public:
	bool raiseInterrupt: 1;
	bool enable: 1;
};

Timer* const timers = (Timer*)0x4000100;

#endif