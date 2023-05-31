/**
 * @file PGA.h
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

#ifndef HEARTRATEMONITOR_PGA_H
#define HEARTRATEMONITOR_PGA_H

#include "stm32g0xx.h"
#include "logger.h"

class PGA {
public:
	typedef enum AddrMode :uint8_t {
		GND = 0x2F,
		VCC = 0x2C,
		NC  = 0x2E
	} AddrMode;
	
	typedef enum Commands : uint8_t {
		NOP = 0b0000,
		writeR = 0b0001,
		readR = 0b0010,
		storeR = 0b0011,
		loadR = 0b0100,
		readNVM = 0b0101,
		writeCtrl = 0b0111,
		readCtrl = 0b1000,
		HALT = 0b1001
	} Commands;

	typedef enum Ctrls : uint8_t {
		enableNVM = 0b001,
		enableWiper = 0b010,
		enableCAL = 0b100
	} Ctrls;

	PGA(uint16_t R_const, uint16_t R_var_max, uint16_t acc, I2C_HandleTypeDef *hi2c, AddrMode addr, Logger log);

	void setMagnification(float m);
	float getMagnification();
	void Inc();
	void Dec();
private:
	I2C_HandleTypeDef *i2c;
	uint8_t addr;

	Logger logger;
	
	uint16_t r_const;
	uint16_t r_var_max;
	uint16_t accuracy;
	
	void sendCommand(Commands comm, uint16_t data);
	void writeWiper(uint16_t wiper);
	uint16_t readWiper();
};

#endif
