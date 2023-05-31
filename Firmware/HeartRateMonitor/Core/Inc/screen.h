/**
 * @file screen.h
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

#ifndef HEARTRATEMONITOR_SCREEN_H
#define HEARTRATEMONITOR_SCREEN_H

#include "stm32g0xx.h"


typedef struct GPIO {
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
} GPIO;


class IO_Pin {
public:
	explicit IO_Pin(GPIO gpio);
	IO_Pin(GPIO_TypeDef* io, uint16_t pinNum);

	void set() const;
	void reset() const;
private:
	GPIO _gpio;
};

class Screen {
public:
	Screen(IO_Pin scl, IO_Pin sda, IO_Pin cs, IO_Pin res, IO_Pin dc, IO_Pin bklt);
	void Init();

	void switchBacklight(bool enable);
	void fillArea(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint16_t color);
	void reset();
	void drawFont(uint16_t x, uint16_t y, const char* str, uint8_t length, uint16_t color, uint16_t background);
private:
	IO_Pin SDA;
	IO_Pin SCL;
	IO_Pin CS;
	IO_Pin RES;
	IO_Pin DC;
	IO_Pin BKLT;

	void spiSend(uint8_t data);
	void sendCommand(uint8_t command);
	void sendData(uint8_t data);
	void sendWdata(uint16_t data);
	void sendWdata(uint16_t *data, size_t size);
	void setRegion(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY);
};

class ScreenFactory {
public:
	ScreenFactory scl(GPIO_TypeDef* gpio, uint16_t pin);
	ScreenFactory sda(GPIO_TypeDef* gpio, uint16_t pin);
	ScreenFactory cs(GPIO_TypeDef* gpio, uint16_t pin);
	ScreenFactory dc(GPIO_TypeDef* gpio, uint16_t pin);
	ScreenFactory res(GPIO_TypeDef* gpio, uint16_t pin);
	ScreenFactory bklt(GPIO_TypeDef* gpio, uint16_t pin);
	Screen build();

private:
	GPIO SDA;
	GPIO SCL;
	GPIO CS;
	GPIO DC;
	GPIO RES;
	GPIO BKLT;
};

#endif //HEARTRATEMONITOR_SCREEN_H
