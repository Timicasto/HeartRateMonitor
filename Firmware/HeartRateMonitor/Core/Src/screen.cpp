#include "screen.h"
#include "numfonts.h"

IO_Pin::IO_Pin(GPIO gpio): _gpio(gpio) {

}

IO_Pin::IO_Pin(GPIO_TypeDef *io, uint16_t pinNum): _gpio({io, pinNum}) {

}

void IO_Pin::set() const {
	HAL_GPIO_WritePin(_gpio.GPIOx, _gpio.GPIO_Pin, GPIO_PIN_SET);
}

void IO_Pin::reset() const {
	HAL_GPIO_WritePin(_gpio.GPIOx, _gpio.GPIO_Pin, GPIO_PIN_RESET);
}

Screen::Screen(SPI_HandleTypeDef *hspi, IO_Pin cs, IO_Pin res, IO_Pin dc, IO_Pin bklt): spi(hspi), CS(cs), RES(res), DC(dc), BKLT(bklt) {
	reset();
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

	// Enable Display
	sendCommand(0x29);
}

void Screen::switchBacklight(bool enable) {
	if (enable) {
		this -> BKLT.set();
	} else {
		this -> BKLT.reset();
	}
}

void Screen::fillArea(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint16_t color) {
	sendCommand(0x2A);
	sendData(0x00);
	sendData(startX + 2);
	sendData(0x00);
	sendData(endX + 2);

	sendCommand(0x2B);
	sendData(0x00);
	sendData(startY + 3);
	sendData(0x00);
	sendData(endY + 3);

	sendCommand(0x2C);

	for (int i = 0; i < (endX - startX); ++i) {
		for (int j = 0; j < (endY - startY); ++j) {
			sendWdata(color);
		}
	}
}

void Screen::reset() {
	this -> RES.reset();
	HAL_Delay(100);
	this -> RES.set();
	HAL_Delay(50);
}

void Screen::drawFont(uint16_t x, uint16_t y, char *str, uint8_t length, uint16_t color) {
	uint16_t currentX = x;
	for (int i = 0 ; i < length ; ++i) {
		setRegion(currentX, y, currentX + 16, y + 30);
		if (str[i] == '.') {
			for (const auto& item : fonts[10]) {
				sendWdata(((item >> 7) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 6) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 5) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 4) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 3) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 2) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 1) & 0x01) ? 0x0000 : color);
				sendWdata((item & 0x01) ? 0x0000 : color);
			}
		} else {
			for (const auto& item : fonts[str[i] - '0']) {
				sendWdata(((item >> 7) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 6) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 5) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 4) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 3) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 2) & 0x01) ? 0x0000 : color);
				sendWdata(((item >> 1) & 0x01) ? 0x0000 : color);
				sendWdata((item & 0x01) ? 0x0000 : color);
			}
		}
	}
}

void Screen::setRegion(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY) {
	sendCommand(0x2A);
	sendData(0x00);
	sendData(startX + 2);
	sendData(0x00);
	sendData(stopX + 2);
	
	sendCommand(0x2B);
	sendData(0x00);
	sendData(startY + 3);
	sendData(0x00);
	sendData(stopY + 3);
	
	sendCommand(0x2C);
}

void Screen::sendCommand(uint8_t command) {
	this -> CS.reset();
	this -> DC.reset();
	HAL_SPI_Transmit(spi, &command, 1, 1000);
	this -> CS.set();
}

void Screen::sendData(uint8_t data) {
	this -> CS.reset();
	this -> DC.set();
	HAL_SPI_Transmit(spi, &data, 1, 1000);
	this -> CS.set();
}

void Screen::sendWdata(uint16_t data) {
	this -> CS.reset();
	this -> DC.set();
	HAL_SPI_Transmit(spi, reinterpret_cast<uint8_t *>(&data), 2, 1000);
	this -> CS.set();
}

void Screen::writeRegister(uint8_t address, uint8_t data) {
	sendCommand(address);
	sendData(data);
}

ScreenFactory ScreenFactory::spi(SPI_HandleTypeDef* hspi) {
	this -> SPI = hspi;
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
	return {this -> SPI, IO_Pin(this -> CS), IO_Pin(this -> RES), IO_Pin(this -> DC), IO_Pin(this -> BKLT)};
}