// Utilities
#include "typedefs.h"
#include "colors.h"
#include "conversions.h"
#include "constants.h"

// CPU
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "timer.h"

// Misc
#include "mem.h"

// Drivers
#include "keyboard.h"
#include "vga_graphics.h"

extern "C" void _start() {
	idt_install();
	isrs_install();
	irq_install();
	asm volatile ("sti");
	timer_install();
	kb_install();
    initializeMem();

	printWithColor(MSG_WELCOME, LIGHT_GREY);

	bool running = true;
	while(running) {
		int x=getCursorX(),y=getCursorY();
		clearScreen(0x00);
		setCursorPosition(x, y);
		drawCharactersToScreen();
		drawCursor();
	}

    return;
}

