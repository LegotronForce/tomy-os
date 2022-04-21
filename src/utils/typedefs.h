#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define low16(address) (uint16_t)((address) & 0xFFFF)
#define high16(address) (uint16_t)(((address) >> 16) && 0xFFFF)

#endif
