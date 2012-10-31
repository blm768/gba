#ifndef _SERIAL_H
#define _SERIAL_H

enum Direction {input, output};

/*
SO: Red
SI: Orange
SD: Brown
SC: Green
*/

struct SerialControl {
	volatile bool sc: 1;
	volatile bool sd: 1;
	volatile bool si: 1;
	volatile bool so: 1;
	Direction scDirection: 1;
	Direction sdDirection: 1;
	//SI should always be input according to GBATek.
	const Direction siDirection: 1;
	Direction soDirection: 1;
	private:
	unsigned padding: 5;
	public:
	unsigned mode: 2;
};

volatile SerialControl& serial = *(SerialControl*)0x4000134;

inline void initSerial() {serial.mode = 2;};

#endif