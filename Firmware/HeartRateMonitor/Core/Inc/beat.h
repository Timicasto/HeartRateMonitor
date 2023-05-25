#ifndef HEARTRATEMONITOR_BEAT_H
#define HEARTRATEMONITOR_BEAT_H

#include "stm32g0xx.h"

class Beat
{
public:
	bool update(uint16_t sample);
	uint8_t getRate();
	void poll();
	
	//MaxDetector detector[8];
	bool isValid = false;
	uint16_t threshold = 65535;
	uint16_t t = 0;

	uint16_t last_beat_s = 0;
	uint16_t last_beat_t = 0;
	uint16_t beat_s = 0;
	uint16_t beat_t = 0;
	uint16_t last_s = 0;

	uint16_t T = 0;
};

#endif
