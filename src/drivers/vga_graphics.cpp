/**
 * @todo
 *  - Make a cursor
 *  - Make the system use ports
 *  - Allow for changing the mode (TEXT, VGA_320_200)
*/

#include "characters.h"
#include "vga_graphics.h"
#include "port_io.h"
#include "colors.h"
#include "typedefs.h"

int textCursor = 0;
int VGACursor[2] = {0, 0};
int lastCursorPosition[2] = {0, 0};
int graphicsMode = GraphicsModes::VGA_320x200;

unsigned char charactersOnScreen[VGA_GRAPHICS_MAX_CHAR_WIDTH][VGA_GRAPHICS_MAX_CHAR_HEIGHT] = {};

/**
 * @brief Set the mode to a specified mode
 * @param mode The mode to set the VGA to
 */
void setMode(GraphicsModes mode) {
    switch(mode) {
        case GraphicsModes::VGA_320x200:
            graphicsMode = GraphicsModes::VGA_320x200;
        break;
    }
}

void setCursorPositionRaw(unsigned short pos) {
    outb(0x3d4, 0x0F);
    outb(0x3d5, (uint8_t)(pos & 0xff));
    outb(0x3d4, 0x0E);
    outb(0x3d5, (uint8_t)((pos >> 8) & 0xff));
}

// TODO: Make this in it's own file (math.h)
int abs(int x) {
    return (x + (x >> 31)) ^ (x >> 31);
}

unsigned char getPixel(int x, int y) {
    if(graphicsMode != GraphicsModes::VGA_320x200) return 0;
    return *(unsigned char *) (VGA_GRAPHICS_ADDRESS + (y * VGA_GRAPHICS_WIDTH + x));
}

void drawPixel(int x, int y, unsigned char color) {
    if(graphicsMode != GraphicsModes::VGA_320x200) return;
    unsigned char *pixel = (unsigned char *) (VGA_GRAPHICS_ADDRESS + (y * VGA_GRAPHICS_WIDTH + x));
    *pixel = color;
}

void drawSquare(int x0, int y0, int x1, int y1, unsigned char color) {
    if(graphicsMode != GraphicsModes::VGA_320x200) return;
    int x, y;
    for(x = x0; x < x1; x++) {
        for(y = y0; y < y1; y++) {
            drawPixel(x, y, color);
        }
    }
}

void drawLine(int x0, int y0, int x1, int y1, unsigned char color) {
    if(graphicsMode != GraphicsModes::VGA_320x200) return;
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
    if(graphicsMode != GraphicsModes::VGA_320x200) {
        *((char*)0xB8000 + (y * 80 + x) * 2) = ch;
        setCursorPositionRaw(textCursor+1);
    } else {
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
}

void drawCharacterAtCursor(char ch, unsigned char color) {
    if(graphicsMode != GraphicsModes::VGA_320x200) {
        if(ch == '\n') {
            setCursorPositionRaw(textCursor + 80 - textCursor % 80);
            textCursor = textCursor + 80 - textCursor % 80;
        } else {
            *((char*)0xB8000 + textCursor * 2) = ch;
            setCursorPositionRaw(textCursor+1);
            textCursor+=1;
        }
    } else {
        if(ch == '\n') {
            VGACursor[0] = 0;
            VGACursor[1] += 8;
        } else {
            if(VGACursor[0]/8 == VGA_GRAPHICS_MAX_CHAR_WIDTH) {
                VGACursor[0] = 0;
                VGACursor[1] += 8;
            }

            drawCharacter(VGACursor[0], VGACursor[1], color, ch);
            VGACursor[0] += 8;
        }
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
    if(graphicsMode != GraphicsModes::VGA_320x200) { textCursor += n; setCursorPositionRaw(textCursor); return; }
    VGACursor[0] += n;
}

void moveCursorUD(int n) {
    if(graphicsMode != GraphicsModes::VGA_320x200) { textCursor += n*80; setCursorPositionRaw(textCursor); return; }
    VGACursor[1] += n*8;
}

void drawCursor() {
    drawCharacter(VGACursor[0], VGACursor[1], DARK_GREY, '_');
}

unsigned char getCurrentCharacter() {
    return charactersOnScreen[VGACursor[0]/8][VGACursor[1]/8];
}

int getCursorX() {
    if(graphicsMode == GraphicsModes::VGA_320x200) return VGACursor[0];
    else if(graphicsMode == GraphicsModes::TEXT_80x25) return textCursor % 80;
    return 0;
}

int getCursorY() {
    if(graphicsMode == GraphicsModes::VGA_320x200) return VGACursor[1];
    else if(graphicsMode == GraphicsModes::TEXT_80x25) return (textCursor - getCursorX()) / 80;
    return 0;
}

void setCursorX(int x) {
    if(graphicsMode == GraphicsModes::VGA_320x200) {
        VGACursor[0] = x;
    } else if(graphicsMode == GraphicsModes::TEXT_80x25) {
        int y = getCursorY();
        textCursor = x + (y * 80);
        setCursorPositionRaw(textCursor);
    }
}

void setCursorY(int y) {
    if(graphicsMode == GraphicsModes::VGA_320x200) VGACursor[1] = y;
    if(graphicsMode == GraphicsModes::TEXT_80x25) {
        int x = getCursorX();
        textCursor = x + y;
        setCursorPositionRaw(textCursor);
    }
}

void setCursorPosition(int x, int y) {
    setCursorX(x);
    setCursorY(y);
}

void drawCharactersToScreen() {
    for(int x=0;x<VGA_GRAPHICS_MAX_CHAR_WIDTH;x++) {
        for(int y=0;y<VGA_GRAPHICS_MAX_CHAR_HEIGHT;y++) {
            if(charactersOnScreen[x][y] != 0) {
                drawCharacter(x*8, y*8, 0x0F, charactersOnScreen[x][y]);
            }
        }
    }
}

void clearScreen(unsigned char color) {
    if(graphicsMode != GraphicsModes::VGA_320x200) {
        for(int i = 0; i < 80*25; i++) {
            *((char*)0xB8000 + i * 2) = ' ';
            *((char*)0xB8000 + i * 2 + 1) = color;
        }
        setCursorPositionRaw(0);
        textCursor = 0;
    } else {
        for(int i = 0; i < VGA_GRAPHICS_MAX_CHAR_WIDTH; i++) {
            for(int j = 0; j < VGA_GRAPHICS_MAX_CHAR_HEIGHT; j++) {
                charactersOnScreen[i][j] = 0;
            }
        }
        VGACursor[0] = 0;
        VGACursor[1] = 0;
    }
}