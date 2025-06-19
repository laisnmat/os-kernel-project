#include "scheduler.h"

#include "process.h"
#include <queue.h>

#include <stdlib.h>
#include "boot/processor_structs.h"

void wakeup_processes()
{
    process_t *p_sleep = queue_top(&head_sleeping, process_t, next);
    while (p_sleep != NULL && p_sleep->wakeup_time <= current_clock())
    {
        p_sleep->state = STATE_READY;
        queue_del(p_sleep, next);
        queue_add(p_sleep, &head_ready, process_t, next, prio);
        p_sleep = queue_top(&head_sleeping, process_t, next);
    }
}

void schedule(void)
{
    //si un processus s'est auto-tué, et qu'il n'est pas en train de run alors on le destroy
    if (process_self_killed != NULL && process_self_killed != running_process) {
        destroy(process_self_killed);
        process_self_killed = NULL;
    }
    wakeup_processes();
    process_t *next_process = queue_top(&head_ready, process_t, next);
    if (next_process == NULL || (running_process->state == STATE_RUNNING && next_process->prio <
                                 running_process->prio))
    {
        // Si aucun processus prêt ou si le prochain processus est déjà prêt moins prio
        return;
    }
    process_t *current_p = running_process;
    next_process = queue_out(&head_ready, process_t, next);
    if
    (current_p->state == STATE_RUNNING)
    {
        current_p->state = STATE_READY;
        queue_add(current_p, &head_ready, process_t, next, prio);
    }
    next_process->state = STATE_RUNNING;
    running_process = next_process;
    tss.esp0 = (int)&next_process->execution_stack[KERNEL_STACK_SIZE];
    ctx_sw(current_p->regs, next_process->regs);
}
