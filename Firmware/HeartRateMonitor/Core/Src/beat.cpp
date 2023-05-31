/**
 * @file beat.cpp
 *
 * @attention
 * <h2> Copyright (C) 2023 Yao Haizhou </h2>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General License for more details.
 *
 * You should have received a copy of the GNU Affero General License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* SPDX-License-Identifier: AGPL-3.0-or-later */

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
		if (sample >= threshold) {
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
		++r;
		time += T;
	}
	return r;
}

