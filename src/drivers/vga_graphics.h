#ifndef VGA_GRAPHICS_H
#define VGA_GRAPHICS_H

// Constant stuffs
#define VGA_TEXT_ADDRESS 0xB8000
#define VGA_GRAPHICS_ADDRESS (char *)0xA0000
#define VGA_GRAPHICS_WIDTH 320
#define VGA_GRAPHICS_HEIGHT 200
#define VGA_GRAPHICS_MAX_CHAR_WIDTH 40
#define VGA_GRAPHICS_MAX_CHAR_HEIGHT 25

unsigned char getPixel(int x, int y);
void drawPixel(int x, int y, unsigned char color);
void drawLine(int x0, int y0, int x1, int y1, unsigned char color);
void drawSquare(int x0, int y0, int x1, int y1, unsigned char color);
void drawCharacter(int x, int y, unsigned char color, char ch);
void drawCharacterAtCursor(char ch, unsigned char color);
void drawString(const char* string, unsigned char color);
void print(const char* string);
void printWithColor(const char* string, unsigned char color);
void moveCursorLR(int n);
void moveCursorUD(int n);
void drawCursor();
unsigned char getCurrentCharacter();
int getCursorX();
int getCursorY();
void setCursorX(int x);
void setCursorY(int y);
void setCursorPosition(int x, int y);
#endif