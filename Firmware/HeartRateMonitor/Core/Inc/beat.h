/**
 * @file beat.h
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

#ifndef HEARTRATEMONITOR_BEAT_H
#define HEARTRATEMONITOR_BEAT_H

#include "stm32g0xx.h"

class Beat
{
public:
	bool update(uint16_t sample);
	uint8_t getRate();
	void poll();
	
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
