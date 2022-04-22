#ifndef PORT_IO_H
#define PORT_IO_H

#define PORT1 0x3F8

void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);

void outl(unsigned short port, unsigned int data);
unsigned int inl(unsigned short port);

#endif
