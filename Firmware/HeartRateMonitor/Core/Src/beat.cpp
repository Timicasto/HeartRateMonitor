#include "beat.h"

void Beat::update(uint16_t s) {
    if (c == 1999) {
        c = 0;
        max = 0;
    }
    if (s >= max) {
        max = s;
        beat = c;
        down_flag = 1;
        stop = 0;
    }
    else {
        if (stop == 0) {
            down_flag = down_flag << 1;
            if (down_flag > 0x0F) {
                down_flag = 1;
                if (beat < last_beat) {
                    rate = 2000 - last_beat + beat;
                } else {
                    rate = beat - last_beat;
                }
                last_beat = beat;
                max = max >> 5;
                stop = 1;
				valid = 1;
            }
        
        }
    }
    c++;
}

uint8_t Beat::getRate() {
    uint8_t r = 0;
    uint16_t counter = 0;
    while (counter < 60000) {
        counter += rate;
        r++;
    }
	valid = 0;
    return r;
}

uint8_t Beat::isValid() const {
	return valid;
}
