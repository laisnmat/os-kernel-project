#include "debugger.h"
#include "display.h"
#include "clock.h"
#include "process.h"
#include "msg_queue.h"

void  auto_test(void);

int helloworld1(void *arg)
{
    if (true)
    {
        printf("Hello world 1 : avec exit et args : %s\n", (char *)arg);
        exit(1);
    }
    return -1;
}

int helloworld3(void *arg)
{
    printf("Hello world 3\n");
    return (int)arg;
}

int helloworld2(void *arg)
{
    printf("Hello world 2 : Je vais m'endormir\n");
    printf("pid start hw1 : %d\n",
           start(helloworld1, MAX_STACK_SIZE, 255, "HW1", "child1 of helloworld2"));
    printf("pid start hw3 : %d\n", start(helloworld3, MAX_STACK_SIZE, 10, "HW3", (void *)2));
    wait_clock(300); // Attendre 100 ticks
    printf("Hello world 2 : Je me suis réveillé\n");
    return (int)arg;
}


int start_test(void *arg)
{
    auto_test();
    return (int)arg;
}

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
    init_traitant_IT(32, traitant_IT_32);
    masque_IRQ(0, 0);
    init_msg_queue_array();
    start_idle();
    start(start_test, 4000, 128, "StartTest", NULL);
    idle();
    while (1)
        hlt();
    return;
}
