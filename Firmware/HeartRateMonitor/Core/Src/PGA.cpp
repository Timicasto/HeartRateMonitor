#include "PGA.h"

PGA::PGA(uint16_t R_const, uint16_t R_var_max, uint16_t acc, I2C_HandleTypeDef *hi2c, AddrMode addr, Logger log)
: r_const(R_const), r_var_max(R_var_max), accuracy(acc), i2c(hi2c), addr(addr), logger(log) {
	sendCommand(Commands::writeCtrl, Ctrls::enableWiper);
	logger.log(Logger::INFO, "PGA Initialized.");
}

void PGA::setMagnification(float m) {
	wiper = m * (r_const / r_var_max * accuracy);
	writeWiper(wiper);
}

float PGA::getMagnification() {
	wiper = readWiper();
	return wiper * (r_var_max / accuracy / r_const);
}

void PGA::Inc() {
	wiper = readWiper();
	if(wiper >= accuracy -1) {
		logger.log(Logger::WARN, "Wiper reached MAX.");
	}
	wiper++;
	writeWiper(wiper);
}

void PGA::Dec() {
	wiper = readWiper();
	if(wiper == 0) {
		logger.log(Logger::WARN, "Wiper reached MIN.");
	}
	wiper--;
	writeWiper(wiper);
}

void PGA::sendCommand(Commands comm, uint16_t data) {
	uint8_t buf[2] = {static_cast<uint8_t>((comm << 2) | ((data >> 8) & 0x3)), static_cast<uint8_t>(data & 0xFF)};
	HAL_I2C_Master_Transmit(i2c, addr, buf, 2, 1000);
}

void PGA::writeWiper(uint16_t wiper) {
	if(wiper >= accuracy) {
		logger.log(Logger::WARN, "Too high wiper, write failed.");
		return;
	}
	sendCommand(Commands::writeR, wiper);
}

uint16_t PGA::readWiper() {
	sendCommand(Commands::readR, 0x00);
	uint8_t buf[2];
	HAL_I2C_Master_Receive(i2c, addr, buf, 2, 1000);
	if(buf[0] >> 6 !=0) {
		logger.log(Logger::ERR, "Wiper format error.");
	}
	uint16_t wiper = ((buf[0] & 0xC3) << 8) | (buf[1] & 0xFF);
	return wiper;
}
