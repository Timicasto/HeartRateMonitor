#ifndef HEARTRATEMONITOR_AD5272_H
#define HEARTRATEMONITOR_AD5272_H

#include "stm32g0xx.h"
#include "logger.h"

class AD5272 {
public:
	typedef enum AD5272AddrMode : uint8_t {
		GND = 0x2F,
		VCC = 0x2C,
		NC  = 0x2E
	} AD5272AddrMode;
	
	typedef enum AD5272Commands : uint8_t {
	
	} AD5272Commands;
	
	AD5272(AD5272AddrMode addrMode, I2C_HandleTypeDef* hi2c, Logger log);
	
private:
	uint8_t address;
	I2C_HandleTypeDef* i2c;
	Logger logger;
};

#endif //HEARTRATEMONITOR_AD5272_H
