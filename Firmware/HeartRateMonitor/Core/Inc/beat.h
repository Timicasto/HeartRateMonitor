#ifndef HEARTRATEMONITOR_BEAT_H
#define HEARTRATEMONITOR_BEAT_H

#include "stm32g0xx.h"
class MaxDetector {
public:
	bool update(uint16_t sample, uint16_t t);
	uint16_t getSample();
	uint16_t getTime();
	uint16_t getDeltaTime();
private:
	uint16_t max_s = 0;
	uint16_t max_t = 0;
	uint16_t s = 0;
	uint16_t t = 0;
	uint16_t last_max_s = 0;
	uint16_t last_max_t = 0;
};

class Beat
{
public:
	bool update(uint16_t sample);
	uint8_t getRate();
private:
	MaxDetector detector[8];
	
	uint16_t t = 0;
	
	uint16_t last_beat_s = 0;
	uint16_t last_beat_t = 0;
	uint16_t beat_s = 0;
	uint16_t beat_t = 0;
	
	uint16_t T = 0;
};

#endif
