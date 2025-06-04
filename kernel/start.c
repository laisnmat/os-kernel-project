#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "display.h"
#include "clock.h"

void kernel_start(void)
{
	efface_ecran();
    printf("HELLO WORLD !");
	unsigned long quartz, ticks;
	clock_settings(&quartz, &ticks);
	init_traitant_IT(32, traitant_IT_32);
	masque_IRQ(0, 0);
	sti();
    while(1)
	  hlt();
	return;
}
