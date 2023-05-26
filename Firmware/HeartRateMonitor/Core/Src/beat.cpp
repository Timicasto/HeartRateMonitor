#include "beat.h"

bool Beat::update(uint16_t sample) {
	if (threshold == 65535) {
		if (sample >= beat_s) {
			beat_t = t;
			beat_s = sample;
		}
		if (sample == 0 && last_s != 0) {
			threshold = beat_s >> 1;
		}
		beat_s = 0;
		last_s = 0;
	} else {
		if (sample >= 40) {
			if (sample >= beat_s) {
				beat_t = t;
				beat_s = sample;
			}
		}
		
		if (sample == 0) {
			if (last_s != 0) {
				poll();
				last_beat_t = beat_t;
				last_beat_s = beat_s;
				beat_s = 0;
			}
		}
	}
	
	++t;
	last_s = sample;
	
	return isValid;
}

void Beat::poll() {
	if (beat_t != last_beat_t) {
		T = beat_t - last_beat_t;
		isValid = true;
	}
}

uint8_t Beat::getRate() {
	isValid = false;
	if(T == 0) {
		return 0;
	}
	uint8_t r = 0;
	uint16_t time = 0;
	while(time < 60000)
	{
		r = r + 1;
		time = time + T;
	}
	return r;
}

