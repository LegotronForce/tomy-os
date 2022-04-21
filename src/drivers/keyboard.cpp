#include "../utils/typedefs.h"
#include "../utils/conversions.h"
// #include "../screen.h"

#include "./vga_graphics.h"
#include "port_io.h"

#include "../cpu/irq.h"
#include "../utils/colors.h"
#include "../utils/string.h"

/********************FUNCTIONS*********************
* kb_install: installs keyboard IRQ handler       *
* keyboard_handler: handles interrupt requests    *
**************************************************/

char kbd_US [128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
    '\t', /* <-- Tab */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
    0, /* <-- control key */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
    '*',
    0,  /* Alt */
    ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

char kbd_US_caps [128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',   
    '\t', /* <-- Tab */
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',     
    0, /* <-- control key */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '|', '~',  0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0,
    '*',
    0,  /* Alt */
    ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

bool shift_pressed = false;
bool caps_lock = false;

void keyboard_handler(struct regs *r) {
    unsigned char scancode;
    scancode = inb(0x60);
    
    /* If the top bit of the byte we read from the keyboard is set, that means that a key has just been released */
    if (scancode & 0x80) {
        /* You can use this one to see if the user released the shift, alt, or control keys... */
        switch(scancode){
            case 0xaa: shift_pressed = false; break;
        }

    } else {
        switch(scancode) {
        case 0x4b: if(getCursorX()/8 <= 0) { setCursorX(VGA_GRAPHICS_WIDTH); setCursorY(getCursorY() - 8); } else { moveCursorUD(-1); } break;
        case 0x4d: if(!(getCursorX()/8 >= VGA_GRAPHICS_MAX_CHAR_WIDTH)) { moveCursorUD(1); } else { setCursorX(VGA_GRAPHICS_WIDTH); setCursorY(getCursorY() - 8); }; break;
        case 0x48: moveCursorUD(-1); break;
        case 0x50: moveCursorUD(1); break;
        case 0x2a: shift_pressed = true; break;
        case 0x3a: caps_lock = !caps_lock; break;
        case 0x0E: // Backspace
            if((getCursorX())/8 <= 0) {
                setCursorX(320);
                setCursorY(getCursorY() - 8);
            } else {
                moveCursorLR(-1);
                drawCharacterAtCursor(0x08, 0x0F);
                moveCursorLR(-1);
            }
        break;

        default:
            // Capital letters VS lowercase letters
            if(shift_pressed || caps_lock) {
                drawCharacterAtCursor(kbd_US_caps[scancode], 0x0F);
            } else {
                drawCharacterAtCursor(kbd_US[scancode], 0x0F);
            }
    	}
        int x = getCursorX(), y = getCursorY();
        setCursorPosition(16, 32);
        print("X");
        print(toString(x, 16));
        print(" Y");
        print(toString(y, 16));
        setCursorPosition(x, y);
    }
}

void kb_install() {
	irq_install_handler(1, keyboard_handler);
}