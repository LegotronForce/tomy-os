#include "../utils/typedefs.h"

/**************** SEND/RECEIVE BYTE ******************/
void outb(uint16_t port, uint8_t data){
	asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
	return;
}

uint8_t inb(uint16_t port){
	uint8_t res;
	asm volatile("inb %1, %0" : "=a"(res) : "Nd"(port));
	return res;
}

void outl(unsigned short port, unsigned int data) {
	asm volatile("outl %0, %1" : : "a"(data), "Nd"(port));
	return;
}

unsigned int inl(unsigned short port) {
	unsigned int res;
	asm volatile("inl %1, %0" : "=a"(res) : "Nd"(port));
	return res;
}