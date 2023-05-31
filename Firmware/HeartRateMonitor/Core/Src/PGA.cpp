/**
 * @file PGA.cpp
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

#include "PGA.h"

PGA::PGA(uint16_t R_const, uint16_t R_var_max, uint16_t acc, I2C_HandleTypeDef *hi2c, AddrMode addr, Logger log) : i2c(hi2c),
addr(addr), logger(log), r_const(R_const), r_var_max(R_var_max), accuracy(acc) {
	sendCommand(Commands::writeCtrl, Ctrls::enableWiper);
	logger.log(Logger::INFO, "PGA Initialized.");
}

void PGA::setMagnification(float m) {
	uint16_t wiper = m * (r_const / r_var_max * accuracy);
	writeWiper(wiper);
}

float PGA::getMagnification() {
	uint16_t wiper = readWiper();
	return wiper * (r_var_max / accuracy / r_const);
}

void PGA::Inc() {
	uint16_t wiper = readWiper();
	if(wiper >= accuracy -1) {
		logger.log(Logger::WARN, "Wiper reached MAX.");
	}
	wiper++;
	writeWiper(wiper);
}

void PGA::Dec() {
	uint16_t wiper = readWiper();
	if(wiper == 0) {
		logger.log(Logger::WARN, "Wiper reached MIN.");
	}
	wiper--;
	writeWiper(wiper);
}

void PGA::sendCommand(Commands comm, uint16_t data) {
	uint8_t buf[2] = {static_cast<uint8_t>((comm << 2) | ((data >> 8) & 0x3)), static_cast<uint8_t>(data & 0xFF)};
	HAL_I2C_Master_Transmit(i2c, addr, buf, 2, 1000);
}

void PGA::writeWiper(uint16_t wiper) {
	if(wiper >= accuracy) {
		logger.log(Logger::WARN, "Too high wiper, write failed.");
		return;
	}
	sendCommand(Commands::writeR, wiper);
}

uint16_t PGA::readWiper() {
	sendCommand(Commands::readR, 0x00);
	uint8_t buf[2];
	HAL_I2C_Master_Receive(i2c, addr, buf, 2, 1000);
	if(buf[0] >> 6 !=0) {
		logger.log(Logger::ERR, "Wiper format error.");
	}
	uint16_t wiper = ((buf[0] & 0xC3) << 8) | (buf[1] & 0xFF);
	return wiper;
}
