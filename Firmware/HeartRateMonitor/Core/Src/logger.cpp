#include "logger.h"

Logger::Logger(UART_HandleTypeDef* huart): uart(huart) {
/*
	std::string str = "Initializing Logger with speed " + std::to_string(huart->Init.BaudRate);
	HAL_UART_Transmit(huart, reinterpret_cast<const uint8_t*>(str.c_str()), str.length(), 1000);
*/
}

/*void Logger::log(Logger::LogLevel level, const std::string& msg) {
	auto time = HAL_GetTick();
	auto levelstr = (level & 0x01) ? "INFO] " : (level >> 1 & 0x01) ? "WARN] " : (level >> 2 & 0x01) ? "ERROR]" : "FATAL]";
	std::string out = "[" + std::to_string(time) + "] [" + levelstr + " " + msg;
	HAL_UART_Transmit(uart, reinterpret_cast<const uint8_t*>(out.c_str()), out.length(), 1000);
}*/
