#include "process.h"

process *process_array[NBPROC];
process *running_process = NULL;
LIST_HEAD(head_ready);
LIST_HEAD(head_sleeping);

void start_idle()
{
    const int32_t pid = 0;
    const char name[PROCESS_NAME_MAX] = "idle";
    const State state = STATE_RUNNING;
    process_array[pid] = mem_alloc(sizeof(process));
    process *idle = process_array[pid];
    idle->pid = pid;
    strcpy(idle->name, name);
    idle->state = state;
    idle->prio = 1;
    running_process = idle;
}


void exit(int retval)
{
    if (running_process != NULL)
    {
        running_process->state = STATE_ZOMBIE;
        running_process->retval = retval;
    }
    schedule();
    assert(false, "Exit called with no running process");
}


int start(int (*pt_func)(void *), unsigned long ssize, int prio, const char *name, void *arg)
{
    uint32_t pid = 0;
    while (pid < NBPROC && process_array[pid] != NULL)
    {
        pid++;
    }
    if (pid >= NBPROC)
    {
        return -1;
    }
    process_array[pid] = mem_alloc(sizeof(process));
    process *current_p = process_array[pid];
    current_p->pid = pid;
    strcpy(current_p->name, name);
    current_p->state = STATE_READY;
    current_p->execution_stack = mem_alloc(ssize * sizeof(int32_t));
    current_p->execution_stack[ssize - 3] = (int32_t)pt_func;
    current_p->execution_stack[ssize - 2] = (int32_t)exit;
    current_p->execution_stack[ssize - 1] = (int32_t)arg;
    /* Execution stack layout:
     *  +--------------------+
     *  |       arg          | ssize - 1
     *  +--------------------+
     *  *       exit         | ssize - 2
     *  +--------------------+
     *  |     call pt_func   | ssize - 3
     *  +--------------------+ <-- %esp
     */
    current_p->regs[ESP] = (int32_t)&current_p->execution_stack[ssize - 3];
    current_p->prio = prio;
    queue_add(current_p, &head_ready, process, next, prio);
    return pid;
}

int getprio(int pid)
{
    // Processus n'existe pas
    if (pid < 0 || pid >= NBPROC || process_array[pid] == NULL)
    {
        return -1;
    }
    return process_array[pid]->prio;
}

int chprio(int pid, int newprio)
{
    // Processus n'existe pas ou priorité invalide
    process *p = process_array[pid];
    if (pid < 0 || pid >= NBPROC || p == NULL || newprio <= 0 || newprio > MAXPRIO)
    {
        return -1;
    }
    int oldprio = p->prio;
    p->prio = newprio;
    // Repositionner le processus dans la file d'attente si nécessaire
    queue_del(p, next);
    queue_add(p, &head_ready, process, next, prio);
    return oldprio;
}

int getpid(void)
{
    return running_process->pid;
}


void wait_clock(unsigned long clock)
{
    process *current_p = running_process;
    current_p->state = STATE_SLEEPING;
    current_p->wakeup_time = clock + current_clock();
    queue_add_by_time(current_p, &head_sleeping, process, next, wakeup_time);
    schedule();
}
