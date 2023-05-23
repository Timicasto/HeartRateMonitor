#ifndef HEARTRATEMONITOR_PGA_H
#define HEARTRATEMONITOR_PGA_H

#include "stm32g0xx.h"
#include "logger.h"

class PGA {
public:
	typedef enum AddrMode :uint8_t {
		GND = 0x2F,
		VCC = 0x2C,
		NC  = 0x2E
	} AddrMode;
	
	typedef enum Commands : uint8_t {
		NOP = 0b0000,
		writeR = 0b0001,
		readR = 0b0010,
		storeR = 0b0011,
		loadR = 0b0100,
		readNVM = 0b0101,
		writeCtrl = 0b0111,
		readCtrl = 0b1000,
		HALT = 0b1001
	} Commands;

	typedef enum Ctrls : uint8_t {
		enableNVM = 0b001,
		enableWiper = 0b010,
		enableCAL = 0b100
	} Ctrls;

	PGA(uint16_t R_const, uint16_t R_var_max, uint16_t acc, I2C_HandleTypeDef *hi2c, AddrMode addr, Logger log);

	void setMagnification(float m);
	float getMagnification();
	void Inc();
	void Dec();
private:
	I2C_HandleTypeDef *i2c;
	uint8_t addr;

	Logger logger;
	
	uint16_t r_const;
	uint16_t r_var_max;
	uint16_t accuracy;
	
	void sendCommand(Commands comm, uint16_t data);
	void writeWiper(uint16_t wiper);
	uint16_t readWiper();
};

#endif
