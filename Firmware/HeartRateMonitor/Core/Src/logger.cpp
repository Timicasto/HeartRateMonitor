#include "printf.h"
#include "logger.h"

Logger::Logger(UART_HandleTypeDef* huart): uart(huart) {
	const char* out = "Initializing Logger with speed";
	HAL_UART_Transmit(huart, reinterpret_cast<const uint8_t *>(out), strlen(out), 1000);
}

void Logger::log(Logger::LogLevel level, const char* msg) {
	auto time = HAL_GetTick();
	auto levelstr = (level & 0x01) ? "INFO] " : (level >> 1 & 0x01) ? "WARN] " : (level >> 2 & 0x01) ? "ERROR]" : "FATAL]";
	size_t size = 24 + strlen(msg);
	char out[size];
	sprintf(out, "%s%lu%s%s%s%s\n", "[", time, "] [", levelstr, " ", msg);
	HAL_UART_Transmit(uart, reinterpret_cast<const uint8_t *>(out), size, 1000);
}
