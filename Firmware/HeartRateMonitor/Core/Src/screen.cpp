/**
 * @file screen.cpp
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

#include "screen.h"
#include "numfonts.h"
#include "color.h"

IO_Pin::IO_Pin(GPIO gpio): _gpio(gpio) {

}

void IO_Pin::set() const {
	HAL_GPIO_WritePin(_gpio.GPIOx, _gpio.GPIO_Pin, GPIO_PIN_SET);
}

void IO_Pin::reset() const {
	HAL_GPIO_WritePin(_gpio.GPIOx, _gpio.GPIO_Pin, GPIO_PIN_RESET);
}

void Screen::switchBacklight(bool enable) {
	if (enable) {
		this -> BKLT.set();
	} else {
		this -> BKLT.reset();
	}
}

Screen::Screen(IO_Pin scl, IO_Pin sda, IO_Pin cs, IO_Pin res, IO_Pin dc, IO_Pin bklt) : SDA(sda), SCL(scl), CS(cs), RES(res), DC(dc), BKLT(bklt){
	reset();
	Init();
}

void Screen::fillArea(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint16_t color) {

	size_t size = (endX-startX)*(endY-startY);
	uint16_t buffer[size];
	
	for (auto &item: buffer) {
		item = (color >> 8) | (color << 8);
	}

	setRegion(startX, startY, endX, endY);
	sendWdata(buffer, size);
	sendCommand(0x29);
}

void Screen::reset() {
	this -> RES.reset();
	HAL_Delay(100);
	this -> RES.set();
	HAL_Delay(120);
}

void Screen::drawFont(uint16_t x, uint16_t y, const char *str, uint8_t length, uint16_t color, uint16_t background) {
	uint16_t currentX = x;
	
	color = (color>>8)|(color<<8);
	background = (background>>8)|(background<<8);
	
	for (int j = 0 ; j < length ; ++j) {
		
		uint16_t bmp[480];
		if (str[j] == '.') {
			for (uint16_t i = 0; i < 60; i++) {
				bmp[i*8  ] = (fonts[10][i] >> 7 & 0x01) ? background : color;
				bmp[i*8+1] = (fonts[10][i] >> 6 & 0x01) ? background : color;
				bmp[i*8+2] = (fonts[10][i] >> 5 & 0x01) ? background : color;
				bmp[i*8+3] = (fonts[10][i] >> 4 & 0x01) ? background : color;
				bmp[i*8+4] = (fonts[10][i] >> 3 & 0x01) ? background : color;
				bmp[i*8+5] = (fonts[10][i] >> 2 & 0x01) ? background : color;
				bmp[i*8+6] = (fonts[10][i] >> 1 & 0x01) ? background : color;
				bmp[i*8+7] = (fonts[10][i]      & 0x01) ? background : color;
			}
		} else {
			for (uint16_t i = 0; i < 60; i++) {
				bmp[i*8  ] = (fonts[str[j] - '0'][i] >> 7 & 0x01) ? background : color;
				bmp[i*8+1] = (fonts[str[j] - '0'][i] >> 6 & 0x01) ? background : color;
				bmp[i*8+2] = (fonts[str[j] - '0'][i] >> 5 & 0x01) ? background : color;
				bmp[i*8+3] = (fonts[str[j] - '0'][i] >> 4 & 0x01) ? background : color;
				bmp[i*8+4] = (fonts[str[j] - '0'][i] >> 3 & 0x01) ? background : color;
				bmp[i*8+5] = (fonts[str[j] - '0'][i] >> 2 & 0x01) ? background : color;
				bmp[i*8+6] = (fonts[str[j] - '0'][i] >> 1 & 0x01) ? background : color;
				bmp[i*8+7] = (fonts[str[j] - '0'][i]      & 0x01) ? background : color;
			}
		}
		setRegion(currentX, y, currentX + 16, y + 30);
		sendWdata(bmp, 480);
		currentX += 16;
	}
	sendCommand(0x29);
}

void Screen::setRegion(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY) {
	sendCommand(0x2A);
	sendData(0x00);
	sendData(startX + 2);
	sendData(0x00);
	sendData(stopX + 1);
	
	sendCommand(0x2B);
	sendData(0x00);
	sendData(startY + 3);
	sendData(0x00);
	sendData(stopY + 2);
	
	sendCommand(0x2C);
}

void Screen::sendCommand(uint8_t command) {
	this -> CS.reset();
	this -> DC.reset();
	spiSend(command);
	this -> CS.set();
}

void Screen::sendData(uint8_t data) {
	this -> CS.reset();
	this -> DC.set();
	spiSend(data);
	this -> CS.set();
}

void Screen::spiSend(uint8_t data) {
	uint8_t i = 0;
	for (i = 8 ; i > 0 ; i--) {
		if (data & 0x80) {
			SDA.set();
		} else {
			SDA.reset();
		}
		SCL.reset();
		SCL.set();
		data <<= 1;
	}
}

void Screen::sendWdata(uint16_t data) {
	this -> CS.reset();
	this -> DC.set();
	data = (data<<8)|(data >>8);
	spiSend(data >> 8);
	spiSend(data);
	this -> CS.set();
}

void Screen::sendWdata(uint16_t *data, size_t size) {
	this -> CS.reset();
	this -> DC.set();
	for (int i = 0; i < size; ++i) {
		sendWdata(data[i]);
	}
	this -> CS.set();	
}

void Screen::Init() {
	sendCommand(0x11); // Exit sleep mode.

	HAL_Delay(120);

	// Frame Rate Setting
	sendCommand(0xB1);
	sendData(0x01);
	sendData(0x2C);
	sendData(0x2D);

	sendCommand(0xB2);
	sendData(0x01);
	sendData(0x2C);
	sendData(0x2D);

	sendCommand(0xB3);
	sendData(0x01);
	sendData(0x2C);
	sendData(0x2D);
	sendData(0x01);
	sendData(0x2C);
	sendData(0x2D);

	// Column Inversion
	sendCommand(0xB4);
	sendData(0x07);

	// Power Sequence
	sendCommand(0xC0);
	sendData(0xA2);
	sendData(0x02);
	sendData(0x84);
	sendCommand(0xC1);
	sendData(0xC5);

	sendCommand(0xC2);
	sendData(0x0A);
	sendData(0x00);

	sendCommand(0xC3);
	sendData(0x8A);
	sendData(0x2A);
	sendCommand(0xC4);
	sendData(0x8A);
	sendData(0xEE);

	sendCommand(0xC5); //VCOM 
	sendData(0x0E);

	sendCommand(0x36); //MX, MY, RGB mode 
	sendData(0xC8);

	// Gamma Sequence
	sendCommand(0xe0);
	sendData(0x0f);
	sendData(0x1a);
	sendData(0x0f);
	sendData(0x18);
	sendData(0x2f);
	sendData(0x28);
	sendData(0x20);
	sendData(0x22);
	sendData(0x1f);
	sendData(0x1b);
	sendData(0x23);
	sendData(0x37);
	sendData(0x00);
	sendData(0x07);
	sendData(0x02);
	sendData(0x10);

	sendCommand(0xe1);
	sendData(0x0f);
	sendData(0x1b);
	sendData(0x0f);
	sendData(0x17);
	sendData(0x33);
	sendData(0x2c);
	sendData(0x29);
	sendData(0x2e);
	sendData(0x30);
	sendData(0x30);
	sendData(0x39);
	sendData(0x3f);
	sendData(0x00);
	sendData(0x07);
	sendData(0x03);
	sendData(0x10);

	sendCommand(0x2a);
	sendData(0x00);
	sendData(0x00);
	sendData(0x00);
	sendData(0x7f);

	sendCommand(0x2b);
	sendData(0x00);
	sendData(0x00);
	sendData(0x00);
	sendData(0x9f);

	// Enable Test Command
	sendCommand(0xF0);
	sendData(0x01);

	// Disable Power Saving Mode
	sendCommand(0xF6);
	sendData(0x00);

	// Enter 65K Mode
	sendCommand(0x3A);
	sendData(0x05);

	sendCommand(0x13);
	HAL_Delay(10);
	// Enable Display
	sendCommand(0x29);
	HAL_Delay(10);
}

ScreenFactory ScreenFactory::scl(GPIO_TypeDef *gpio, uint16_t pin) {
	this -> SCL = {gpio, pin};
	return *this;
}

ScreenFactory ScreenFactory::sda(GPIO_TypeDef *gpio, uint16_t pin) {
	this -> SDA = {gpio, pin};
	return *this;
}

ScreenFactory ScreenFactory::cs(GPIO_TypeDef* gpio, uint16_t pin) {
	this -> CS = {gpio, pin};
	return *this;
}

ScreenFactory ScreenFactory::dc(GPIO_TypeDef* gpio, uint16_t pin) {
	this -> DC = {gpio, pin};
	return *this;
}

ScreenFactory ScreenFactory::res(GPIO_TypeDef* gpio, uint16_t pin) {
	this -> RES = {gpio, pin};
	return *this;
}

ScreenFactory ScreenFactory::bklt(GPIO_TypeDef* gpio, uint16_t pin) {
	this -> BKLT = {gpio, pin};
	return *this;
}

Screen ScreenFactory::build() {
	return {IO_Pin(this -> SCL), IO_Pin(this -> SDA), IO_Pin(this -> CS), IO_Pin(this -> RES), IO_Pin(this -> DC), IO_Pin(this -> BKLT)};
}
