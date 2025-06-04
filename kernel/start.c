#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "display.h"

void kernel_start(void)
{
	efface_ecran();
    printf("HELLO WORLD !");
    while(1)
	  hlt();
	return;
}
