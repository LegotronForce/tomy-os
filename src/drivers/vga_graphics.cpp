#include "./characters.h"
#include "./vga_graphics.h"

int VGACursor[2] = {0, 0};
int lastCursorPosition[2] = {0, 0};

unsigned char charactersOnScreen[VGA_GRAPHICS_MAX_CHAR_WIDTH][VGA_GRAPHICS_MAX_CHAR_HEIGHT] = {};

// TODO: Make this in it's own file (math.h)
int abs(int x) {
    return (x + (x >> 31)) ^ (x >> 31);
}

unsigned char getPixel(int x, int y) {
  return *(unsigned char *) (VGA_GRAPHICS_ADDRESS + (y * VGA_GRAPHICS_WIDTH + x));
}

void drawPixel(int x, int y, unsigned char color) {
    unsigned char *pixel = (unsigned char *) (VGA_GRAPHICS_ADDRESS + (y * VGA_GRAPHICS_WIDTH + x));
    *pixel = color;
}

void drawSquare(int x0, int y0, int x1, int y1, unsigned char color) {
    int x, y;
    for(x = x0; x < x1; x++) {
        for(y = y0; y < y1; y++) {
            drawPixel(x, y, color);
        }
    }
}

void drawLine(int x0, int y0, int x1, int y1, unsigned char color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for(;;) {
        drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void drawCharacter(int x, int y, unsigned char color, char ch) {
    // CLEAR_CHARACTER
    if(ch == 0x08) {
        charactersOnScreen[VGACursor[0]/8][VGACursor[1]/8] = 0;
        int i, j;
        for(i = 0; i < 8; i++) {
            for(j = 0; j < 8; j++) {
                drawPixel(x + j, y + i, 0x00);
            }
        }
    } else {
        if(charactersOnScreen[VGACursor[0]/8][VGACursor[1]/8] != ch) {
            charactersOnScreen[VGACursor[0]/8][VGACursor[1]/8] = ch;
            int i, j;
            for(i = 0; i < 8; i++) {
                for(j = 0; j < 8; j++) {
                    drawPixel(x + j, y + i, 0x00);
                }
            }
        }
        
        int i, j;
        for(i = 0; i < 8; i++) {
            for(j = 0; j < 8; j++) {
                if(font8x8_basic[ch][i] & (1 << j)) {
                    drawPixel(x + j, y + i, color);
                }
            }
        }
    }
}

void drawCharacterAtCursor(char ch, unsigned char color) {
    if(ch == '\n') {
        setCursorX(0);
        setCursorY(getCursorY() - 8);
    } else {
        if(VGACursor[0]/8 == VGA_GRAPHICS_MAX_CHAR_WIDTH) {
            VGACursor[0] = 0;
            VGACursor[1] += 8;
        }

        drawCharacter(VGACursor[0], VGACursor[1], color, ch);
        VGACursor[0] += 8;
    }
}

void drawString(const char* string, unsigned char color) {
    int i;
    for(i = 0; string[i] != '\0'; i++) {
        drawCharacterAtCursor(string[i], color);
    }
}

void print(const char* string ) {
    drawString(string, 0x0F);
}

void printWithColor(const char* string, unsigned char color) {
    drawString(string, color);
}

void moveCursorLR(int n) {
    VGACursor[0] += n*8;
}

void moveCursorUD(int n) {
    VGACursor[1] += n*8;
}

void drawCursor() {
    if(VGACursor[0] == lastCursorPosition[0] && VGACursor[1] == lastCursorPosition[1]) { return; }
    drawPixel(lastCursorPosition[0]-7, lastCursorPosition[1]-7, 0x00);
    drawPixel(VGACursor[0]-7, VGACursor[1]-7, 0x06);
    
    lastCursorPosition[0] = VGACursor[0];
    lastCursorPosition[1] = VGACursor[1];
}

unsigned char getCurrentCharacter() {
    return charactersOnScreen[VGACursor[0]/8][VGACursor[1]/8];
}

int getCursorX() {
    return VGACursor[0];
}

int getCursorY() {
    return VGACursor[1];
}

void setCursorX(int x) {
    VGACursor[0] = x;
}

void setCursorY(int y) {
    VGACursor[1] = y;
}

void setCursorPosition(int x, int y) {
    VGACursor[0] = x;
    VGACursor[1] = y;
}