#include "ad5272.h"

AD5272::AD5272(AD5272::AD5272AddrMode addrMode, I2C_HandleTypeDef* hi2c, Logger log): address(addrMode), i2c(hi2c),
                                                                                      logger(log) {
	//logger.log(Logger::INFO, "Initialized AD5272.");
}
