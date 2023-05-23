#include "ad5272.h"

AD5272::AD5272(AD5272::AD5272AddrMode addrMode, I2C_HandleTypeDef* hi2c, Logger log, uint16_t maxR, uint16_t acc): address(addrMode), i2c(hi2c),
                                                                                      logger(log), maxResistance(maxR), accuracy(acc) {
	sendCommandWithData(AD5272Commands::COMM, AD5272Controls::RDAC_WP_REN);
	logger.log(Logger::INFO, "Initialized AD5272.");
}

void AD5272::sendCommand(AD5272::AD5272Commands com) {
	uint8_t data[2] = {static_cast<uint8_t>(com << 2), 0x00};
	HAL_I2C_Master_Transmit(i2c, address, data, 2, 1000);
}

void AD5272::sendCommandWithData(AD5272::AD5272Commands com, uint16_t data) {
	uint8_t buf[2] = {static_cast<uint8_t>((com << 2 | ((data & 0x300) >> 8))), static_cast<uint8_t>(data & 0xFF)};
	HAL_I2C_Master_Transmit(i2c, address, buf, 2, 1000);
}

void AD5272::setResistance(double resistance) {
	auto wiper = (unsigned short)(resistance * accuracy / (double)maxResistance);
	sendCommandWithData(R_SET, wiper);
}

double AD5272::readResistance() {
	sendCommand(R_READ);
	uint8_t buf[2];
	HAL_I2C_Master_Receive(i2c, address, buf, 2, 1000);
	if(buf[0] >> 6 != 0) {
		logger.log(Logger::ERR, "Resistance format error.");
	}
	uint16_t wiper = ((buf[0] & 0xC3) << 8) | (buf[1] & 0xFF);
	return wiper * (maxResistance / (double)accuracy);
}
