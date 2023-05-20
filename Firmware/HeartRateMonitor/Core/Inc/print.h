#ifndef HEARTRATEMONITOR_PRINT_H
#define HEARTRATEMONITOR_PRINT_H

#include "stm32g0xx.h"

class Print {
public:
	Print(char*, size_t);
	Print uint16(uint16_t);
	Print uint8(uint8_t);
	Print string(const char *, size_t);
	Print newline();
	Print carriagereturn();
	int16_t end();
private:
	char* buffer;
	char* next;
	size_t size;
	static void AddNumChars(char *, const char *, size_t);
};

#endif //HEARTRATEMONITOR_PRINT_H
