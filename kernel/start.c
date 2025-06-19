#include "clock.h"
#include "debugger.h"
#include "display.h"
#include "msg_queue.h"
#include "process.h"

#define start_user 0x1000000

void idle()
{
    for (;;)
    {
        sti();
        hlt(); // autre exception possible: le code de vos tests
        cli();
    }
}

void kernel_start(void)
{
    efface_ecran();

    unsigned long quartz, ticks;
    clock_settings(&quartz, &ticks);
    init_traitant_IT(32, traitant_IT_32, 0);
    init_traitant_IT(33, traitant_IT_33, 0);   // handler clavier
    init_traitant_IT(0x49, traitant_IT_49, 3); // handler pour les appels système
    init_traitant_IT(13, traitant_IT_13, 3);
    masque_IRQ(0, 0);
    masque_IRQ(1, 0); // démasque l'IRQ clavier
    init_msg_queue_array();
    start_idle();

    start((int (*)(void *))start_user, 256, 2, "User Proc", NULL);
    idle();
    while (1)
        hlt();
    return;
}
