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
	
	return false;
}

void Beat::poll() {
	if (beat_t != last_beat_t) {
		T = beat_t - last_beat_t;
		isValid = true;
	}
}

uint8_t Beat::getRate() {
	// TODO: Rate calculation
}

