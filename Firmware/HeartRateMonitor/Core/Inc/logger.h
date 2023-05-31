/**
 * @file logger.h
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

#ifndef HEARTRATEMONITOR_LOGGER_H
#define HEARTRATEMONITOR_LOGGER_H

#include <cstring>
#include "stm32g0xx.h"

class Logger {
public:
	
	typedef enum LogLevel : uint8_t {
		INFO = 0b1,
		WARN = 0b10,
		ERR = 0b100,
		FATAL = 0b1000
	} LogLevel;
	
	explicit Logger(UART_HandleTypeDef* huart);
	void log(LogLevel level, const char* msg);
	
private:
	UART_HandleTypeDef* uart;
};

#endif //HEARTRATEMONITOR_LOGGER_H
