// Utilities
#include "utils/typedefs.h"
#include "utils/colors.h"
#include "utils/conversions.h"

// CPU
#include "cpu/idt.h"
#include "cpu/irq.h"
#include "cpu/isr.h"
#include "cpu/timer.h"

// Misc
#include "mem.h"
#include "screen.h"

// Drivers
#include "drivers/keyboard.h"
#include "drivers/vga_graphics.h"

struct VendorStruct {
	int i;
	int j;
	int vendor;
};

extern "C" void _start() {
	idt_install();
	isrs_install();
	irq_install();
	asm volatile ("sti");
	timer_install();
	kb_install();
    initializeMem();

	print("If you can see this then... POGGERS!");

	for(;;) { sleep(1); } //drawCursor();

    return;
}

