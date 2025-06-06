#include "scheduler.h"

#include "process.h"
#include "../shared/queue.h"

#include <stdlib.h>

void schedule(void)
{
    process *p_sleep = queue_top(&head_sleeping, process, next);
    while (p_sleep != NULL && p_sleep->wakeup_time <= current_clock())
    {
        p_sleep->state = STATE_READY;
        int pid = p_sleep->pid;
        queue_del(p_sleep, next);
        queue_add(process_array[pid], &head_ready, process, next, prio);
        p_sleep = queue_top(&head_sleeping, process, next);
    }

    process *next_process = queue_top(&head_ready, process, next);
    if
    (next_process == NULL || (running_process->state == STATE_RUNNING && next_process->prio <
                              running_process->prio))
    {
        // Si aucun processus prêt ou si le prochain processus est déjà prêt moins prio
        return;
    }
    process *current_p = running_process;
    next_process = queue_out(&head_ready, process, next);
    if
    (current_p->state == STATE_RUNNING)
    {
        current_p->state = STATE_READY;
        queue_add(current_p, &head_ready, process, next, prio);
    }
    next_process->state = STATE_RUNNING;
    running_process = next_process;
    ctx_sw(current_p->regs, next_process->regs);
}
