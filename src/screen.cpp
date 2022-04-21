#include "utils/typedefs.h"
#include "utils/string.h"
#include "drivers/port_io.h"
#include "utils/colors.h"
#include "utils/conversions.h"
#include "screen.h"

int cursor = 0;

void setCursorPositionRaw(unsigned short pos) {
    outb(0x3d4, 0x0F);
    outb(0x3d5, (uint8_t)(pos & 0xff));
    outb(0x3d4, 0x0E);
    outb(0x3d5, (uint8_t)((pos >> 8) & 0xff));
}

void setProgramCursor(unsigned short pos) {
    cursor = pos;
}

void setCursorPosition(int x, int y) {
    uint16_t pos = y * 80 + x;
    setCursorPositionRaw(pos);
    cursor = pos;
}

void printChar(char c, char col) {
    // NEWLINES
    if(c == '\n') {
        setCursorPositionRaw(cursor + 80 - cursor % 80);
        cursor = cursor + 80 - cursor % 80;
    } else {
        *((char*)0xB8000 + cursor * 2) = c;
        setCursorPositionRaw(cursor+1);
        cursor+=1;
    }
}

// void kprint(const char* s) {
//     int sLen = strLen(s);

//     for(int i = 0; i < sLen; i++) {
//         kprintChar(s[i], 0x0F);
//     }
// }

void scrollPageUp(){
    for(int i = 160*2; i < 4000 - 160; i++) *((char*)0xB8000 + i - 160) = *((char*)0xB8000 + i);
}

void print(const char* string) {
    uint8_t* charPtr = (uint8_t*)string;
	uint16_t i = cursor;
	while(*charPtr != 0){

        if(i < 1840){
            *((char*)0xB8000 + i * 2) = *charPtr;
            i++;
        }
        else{
            scrollPageUp();
            i = 1760;
        }

	    charPtr++;
	}
	
    setCursorPositionRaw(i+1);
    cursor = i;
	
    return;
}

void moveCursorLR(int i){			// MOVE CURSOR HORIZONTALLY
    cursor += i;
    setCursorPositionRaw(cursor);
	return;
}

void moveCursorUD(int i) {			// MOVE CURSOR VERTICALLY
    cursor += 80 * i;
    setCursorPositionRaw(cursor);
	
    return;
}

void clearScreen(int col) {
    for(int i = (int)(char*)0xB8000; i < (int)(char*)0xB8000 + 4000; i += 1) {
        if(i % 2 == 0) *((char*)i) = ' ';	//Default is spaces
        else *((char*)i) = col;
    }
}

int getCursorPosition() {
    return cursor;
}

// *(char*)(0xB8000 + cursor) = c;
// cursor++;
// *(char*)(0xB8000 + cursor) = col;
// cursor++;