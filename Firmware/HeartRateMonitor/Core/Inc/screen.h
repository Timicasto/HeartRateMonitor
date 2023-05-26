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
//	Screen(SPI_HandleTypeDef* hspi, IO_Pin cs, IO_Pin res, IO_Pin dc, IO_Pin bklt);
	Screen(IO_Pin scl, IO_Pin sda, IO_Pin cs, IO_Pin res, IO_Pin dc, IO_Pin bklt);
	void Init();

	void switchBacklight(bool enable);
	void fillArea(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint16_t color);
	void reset();
	void drawFont(uint16_t x, uint16_t y, const char* str, uint8_t length, uint16_t color, uint16_t background);
	void drawString(const char *);
private:
//	SPI_HandleTypeDef* spi;
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
	void sendWdata(uint16_t data, uint16_t count);
	void writeRegister(uint8_t address, uint8_t data);
	void setRegion(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY);
};

class ScreenFactory {
public:
//	ScreenFactory spi(SPI_HandleTypeDef* hspi);
	ScreenFactory scl(GPIO_TypeDef* gpio, uint16_t pin);
	ScreenFactory sda(GPIO_TypeDef* gpio, uint16_t pin);
	ScreenFactory cs(GPIO_TypeDef* gpio, uint16_t pin);
	ScreenFactory dc(GPIO_TypeDef* gpio, uint16_t pin);
	ScreenFactory res(GPIO_TypeDef* gpio, uint16_t pin);
	ScreenFactory bklt(GPIO_TypeDef* gpio, uint16_t pin);
	Screen build();

private:
//	SPI_HandleTypeDef* SPI;
	GPIO SDA;
	GPIO SCL;
	GPIO CS;
	GPIO DC;
	GPIO RES;
	GPIO BKLT;
};

#endif //HEARTRATEMONITOR_SCREEN_H
