/**
 * @file print.h
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

#ifndef HEARTRATEMONITOR_PRINT_H
#define HEARTRATEMONITOR_PRINT_H

#include "stm32g0xx.h"

class Print {
public:
	Print(char*, size_t);
	Print uint16(uint16_t);
	Print uint8(uint8_t);
	Print string(const char *, size_t);
	Print newline();
	Print carriagereturn();
	int16_t end();
private:
	char* buffer;
	char* next;
	size_t size;
	static void AddNumChars(char *, const char *, size_t);
};

#endif //HEARTRATEMONITOR_PRINT_H
