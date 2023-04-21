#ifndef HEARTRATEMONITOR_LOGGER_H
#define HEARTRATEMONITOR_LOGGER_H

//#include <string>
#include "stm32g0xx.h"

class Logger {
public:
	
	typedef enum LogLevel : uint8_t {
		INFO = 0b1,
		WARN = 0b10,
		ERR = 0b100,
		FATAL = 0b1000
	} LogLevel;
	
	explicit Logger(UART_HandleTypeDef* huart);
//	void log(LogLevel level, const std::string& msg);
	
private:
	UART_HandleTypeDef* uart;
};

#endif //HEARTRATEMONITOR_LOGGER_H
