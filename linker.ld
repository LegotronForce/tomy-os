/* This is my first time using linkerscript, so don't judge! */
SECTIONS {
    .text : { *(.text) }
    .DATA : { [A-Z]*(.data) }
    .data : { *(.data) }
    .rodata : { *(.rodata) }
}