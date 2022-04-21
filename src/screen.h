#ifndef SCREEN_H
#define SCREEN_H

void clearScreen(int col);
void print(const char* string);
void printChar(char c, char col);
void setCursorPositionRaw(unsigned short i);
void setCursorPosition(int x, int y);
void moveCursorLR(int i);
void moveCursorUD(int i);
void scrollPageUp();
void setProgramCursor(unsigned short pos);
int getCursorPosition();

#endif