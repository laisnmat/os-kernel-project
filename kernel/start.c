#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "display.h"
#include "clock.h"
#include "process.h"

void kernel_start(void)
{
    efface_ecran();
    init_idle();
    char nom[PROCESS_NAME_MAX];
    for (int i = 1; i <= NBPROC; i++)
    {
        snprintf(nom, PROCESS_NAME_MAX, "proc%d", i);
        create_process(proc1, nom);
    }
    unsigned long quartz, ticks;
    clock_settings(&quartz, &ticks);
    init_traitant_IT(32, traitant_IT_32);
    masque_IRQ(0, 0);
    idle();
    while (1)
        hlt();
    return;
}
