#include "cpu.h"
#include "kbd.h"
#include "keyboard-glue.h"
#include "params.h"
#include <stdint.h>

void tic_kbd(void)
{
    uint8_t scancode = inb(0x60);
    do_scancode(scancode);
    outb(0x20, 0x20);
}
