#include "scheduler.h"

#include "process.h"
#include "../shared/queue.h"

#include "../shared/stdlib.h"

void clean_zombies_processes()
{
    if (queue_empty(&head_zombie)) return;

    process *zombie = queue_entry(head_zombie.next, process, next);
    while (&zombie->next != &head_zombie)
    {
        process *next_zombie = queue_entry(zombie->next.next, process, next);
        if (zombie->parent == NULL)
        {
            destroy(zombie);
        }
        zombie = next_zombie;
    }
}

void wakeup_processes()
{
    process *p_sleep = queue_top(&head_sleeping, process, next);
    while (p_sleep != NULL && p_sleep->wakeup_time <= current_clock())
    {
        p_sleep->state = STATE_READY;
        queue_del(p_sleep, next);
        queue_add(p_sleep, &head_ready, process, next, prio);
        p_sleep = queue_top(&head_sleeping, process, next);
    }
}

void schedule(void)
{
    clean_zombies_processes();
    wakeup_processes();
    process *next_process = queue_top(&head_ready, process, next);
    if (next_process == NULL || (running_process->state == STATE_RUNNING && next_process->prio <
                                 running_process->prio))
    {
        // Si aucun processus prêt ou si le prochain processus est déjà prêt moins prio
        return;
    }
    process *current_p = running_process;
    next_process = queue_out(&head_ready, process, next);
    if (running_process == NULL)
    {
        running_process = next_process;
        ctx_sw(NULL, next_process->regs);
    }
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
