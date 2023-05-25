#include "print.h"

Print::Print(char *b, size_t s):buffer(b), next(b), size(s) {

}

Print Print::uint16(uint16_t n) {
	char num[5]={'0','0','0','0','0'};
	
	if((n >> 15) & 0x1)
		AddNumChars(num, "32768", 5);
	if((n >> 14) & 0x1)
		AddNumChars(num, "16384", 5);
	if((n >> 13) & 0x1)
		AddNumChars(num, "08192", 5);
	if((n >> 12) & 0x1)
		AddNumChars(num, "04096", 5);
	if((n >> 11) & 0x1)
		AddNumChars(num, "02048", 5);
	if((n >> 10) & 0x1)
		AddNumChars(num, "01024", 5);
	if((n >>  9) & 0x1)
		AddNumChars(num, "00512", 5);
	if((n >>  8) & 0x1)
		AddNumChars(num, "00256", 5);
	if((n >>  7) & 0x1)
		AddNumChars(num, "00128", 5);
	if((n >>  6) & 0x1)
		AddNumChars(num, "00064", 5);
	if((n >>  5) & 0x1)
		AddNumChars(num, "00032", 5);
	if((n >>  4) & 0x1)
		AddNumChars(num, "00016", 5);
	if((n >>  3) & 0x1)
		AddNumChars(num, "00008", 5);
	if((n >>  2) & 0x1)
		AddNumChars(num, "00004", 5);
	if((n >>  1) & 0x1)
		AddNumChars(num, "00002", 5);
	if((n      ) & 0x1)
		AddNumChars(num, "00001", 5);

	bool ishead = true;
	for(char i : num) {
		if((i == '0') & ishead) {
			*(this->next) = ' ';
		} else {
			*(this->next) = i;
			ishead = false;
		}
		this->next++;
	}
	return *this;
}

Print Print::uint8(uint8_t n) {
char num[3]={'0','0','0'};

	if((n >>  7) & 0x1)
		AddNumChars(num, "128", 3);
	if((n >>  6) & 0x1)
		AddNumChars(num, "064", 3);
	if((n >>  5) & 0x1)
		AddNumChars(num, "032", 3);
	if((n >>  4) & 0x1)
		AddNumChars(num, "016", 3);
	if((n >>  3) & 0x1)
		AddNumChars(num, "008", 3);
	if((n >>  2) & 0x1)
		AddNumChars(num, "004", 3);
	if((n >>  1) & 0x1)
		AddNumChars(num, "002", 3);
	if((n      ) & 0x1)
		AddNumChars(num, "001", 3);

	bool ishead = true;
	for(char i : num) {
		if((i == '0') & ishead) {
			*(this->next) = ' ';
		} else {
			*(this->next) = i;
			ishead = false;
		}
		this->next++;
	}
	return *this;
}

Print Print::string(const char *const str, size_t size) {
	const char *p = str;
	while(*p != '\000') {
		*(this->next) = *p;
		this->next++;
	}
	return *this;
}

Print Print::newline() {
	*(this->next) = '\n';
	this->next++;
	return *this;
}

Print Print::carriagereturn() {
	*(this->next) = '\r';
	this->next++;
	return *this;
}

int16_t Print::end() {
	*(this->next) = '\000';
	this->next++;
	return static_cast<int16_t>(this->next - this->buffer - size);
}

void Print::AddNumChars(char *const a,const char *const b, size_t size) {
	char carry = '0';
	size_t i = size;
	while(i > 0) {
		i = i - 1;
		
		*(a + i) += *(b + i) - '0' + carry - '0';
		carry = '0';
		if(*(a + i) > '9') {
			*(a + i) -= '9' - '0' + 1;
			carry += 1;
		}
	}
}










