# VGA_Graphics.h
`vga_graphics.h` provides a simple way to draw shapes and characters in TomyOS. There are quite a few functions and this document is meant to describe it.

## API
| Functions |
| --------- |
| [`unsigned char getPixel(int x, int y)`](#unsigned-char-getpixel(int-x-int-y)) |
| [`void drawPixel(int x, int y, unsigned char color)`]() |
| [`void drawLine(int x0, int y0, int x1, int y1, unsigned char color)`]() |
| [`void drawSquare(int x0, int y0, int x1, int y1, unsigned char color)`]() |
| [`void drawCharacter(int x, int y, char ch)`]() |
| [`void drawCharacterAtCursor(char ch, unsigned char color)`]() |
| [`void drawString(const char* string, unsigned char color)`]() |
| [`void print(const char* string)`]() |
| [`void printWithColor(const char* string, unsigned char color)`]() |
| [`void moveCursorLR(int n)`]() |
| [`void moveCursorUD(int n)`]() |
| [`void drawCursor()`]() |
| [`unsigned char getCurrentCharacter();`]() |
| [`int getCursorX()`]() |
| [`int getCursorY()`]() |

### `unsigned char getPixel(int x, int y)`
