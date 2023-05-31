/**
 * @file logger.cpp
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

#include "printf.h"
#include "logger.h"

Logger::Logger(UART_HandleTypeDef* huart): uart(huart) {
	const char* out = "Initializing Logger with speed";
	HAL_UART_Transmit(huart, reinterpret_cast<const uint8_t *>(out), strlen(out), 1000);
}

void Logger::log(Logger::LogLevel level, const char* msg) {
	auto time = HAL_GetTick();
	auto levelstr = (level & 0x01) ? "INFO] " : (level >> 1 & 0x01) ? "WARN] " : (level >> 2 & 0x01) ? "ERROR]" : "FATAL]";
	size_t size = 25 + strlen(msg);
	char out[size];
	sprintf(out, "%s%lu%s%s%s%s\r\n", "[", time, "] [", levelstr, " ", msg);
	HAL_UART_Transmit(uart, reinterpret_cast<const uint8_t *>(out), strlen(out), 1000);
}
