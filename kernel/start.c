#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "display.h"
#include "clock.h"
#include "process.h"


int helloworld1(void *arg)
{
    printf("Hello world 1 %s\n", (char *)arg);
    return 0;
}

int helloworld2(void *arg)
{
    printf("Hello world 2 : Je vais m'endormir\n");
    wait_clock(300); // Attendre 100 ticks
    printf("Hello world 2 : Je me suis réveillé\n");
    return (int)arg;
}

int helloworld3(void *arg)
{
    printf("Hello world 3\n");
    return (int)arg;
}


void idle()
{
    for (;;)
    {
        sti(); // usage de sti et cli avant la phase 5 (possible après)
        hlt(); // autre exception possible: le code de vos tests
        cli(); // JAMAIS de sti ou de cli dans le reste de votre kernel
    }
}

void kernel_start(void)
{
    efface_ecran();
    start_idle();
    start(helloworld1, STACK_SIZE, 100, "HelloWorld1", "TEST");
    start(helloworld2, STACK_SIZE, 200, "HelloWorld2", NULL);
    start(helloworld3, STACK_SIZE, 50, "HelloWorld3", NULL);

    unsigned long quartz, ticks;
    clock_settings(&quartz, &ticks);
    init_traitant_IT(32, traitant_IT_32);
    masque_IRQ(0, 0);
    idle();
    while (1)
        hlt();
    return;
}
