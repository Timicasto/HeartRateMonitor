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
		NOP = 0b0000,
		R_SET = 0b0001,
		R_READ = 0b0010,
		R_STORE = 0b0011,
		R_RELOAD = 0b0100,
		NVM_READ = 0b0101,
		COMM = 0b0111,
		COMM_READ = 0b1000,
		HALT = 0b1001
	} AD5272Commands;

	typedef enum AD5272Controls : uint8_t {
		NVM_EN = 0b001,
		RDAC_WP_REN = 0b010,
		CAL_REN = 0b100
	} AD5272Operations;
	
	AD5272(AD5272AddrMode addrMode, I2C_HandleTypeDef* hi2c, Logger log, uint16_t maxR, uint16_t acc);

	void sendCommand(AD5272Commands com);
	void setResistance(double resistance);
	double readResistance();
	
private:
	uint8_t address;
	I2C_HandleTypeDef* i2c;
	Logger logger;
	uint16_t accuracy;
	uint16_t maxResistance;

	void sendCommandWithData(AD5272Commands com, uint16_t data);
};

#endif //HEARTRATEMONITOR_AD5272_H
