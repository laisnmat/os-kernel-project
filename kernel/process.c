#include "process.h"

#include "clock.h"
#include "../shared/debug.h"

process *process_array[NBPROC];
process *head_ready_process;
process *tail_ready_process;

process *head_sleeping_process;
process *tail_sleeping_process;

void insert_ready_head(process *new_process)
{
    process *prev_head = head_ready_process;
    head_ready_process = new_process;
    if (prev_head != NULL)
        prev_head->state = STATE_READY;
    new_process->state = STATE_RUNNING;
    //new_process->suiv = prev_head; pour l'instant on ne veut pas que l'ancienne tete soit le suivant mais plutot à la queue
}

void insert_ready_tail(process *new_process)
{
    process *prev_tail = tail_ready_process;
    tail_ready_process = new_process;
    if (prev_tail != NULL)
        prev_tail->suiv = new_process;
    new_process->suiv = NULL;
    new_process->state = STATE_READY;
}

void insert_sleeping_head(process *new_process)
{
    new_process->suiv = head_sleeping_process;
    new_process->state = STATE_SLEEPING;
    head_sleeping_process = new_process;
}

void insert_sleeping_tail(process *new_process)
{
    process *prev_tail = tail_sleeping_process;
    tail_sleeping_process = new_process;
    if (prev_tail != NULL)
        prev_tail->suiv = new_process;
    new_process->suiv = NULL;
    new_process->state = STATE_SLEEPING;
}

void insert_sleeping_by_time(process *new_process)
{
    process *current_process = head_sleeping_process;
    if (current_process == NULL || current_process->time_before_wake_up > new_process->
        time_before_wake_up)
    {
        insert_sleeping_head(new_process);
        return;
    }
    while (current_process->suiv != NULL && current_process->suiv->time_before_wake_up <
           new_process->time_before_wake_up)
    {
        current_process = current_process->suiv;
    }
    if (current_process->suiv == NULL)
    {
        insert_sleeping_tail(new_process);
        return;
    }
    new_process->suiv = current_process->suiv;
    current_process->suiv = new_process;
    new_process->state = STATE_SLEEPING;
}

void sleep(uint32_t seconds)
{
    uint32_t nb_ticks_before_wake_up = seconds * CLOCKFREQ + current_clock();
    head_ready_process->time_before_wake_up = nb_ticks_before_wake_up;
    process *prev_head_ready = head_ready_process;
    process *new_head_ready = head_ready_process->suiv;
    insert_sleeping_by_time(head_ready_process);
    head_ready_process = new_head_ready;
    wait_clock(nb_ticks_before_wake_up);
    //enlever dans liste des waiting
    head_sleeping_process = head_sleeping_process->suiv;
    //inserer dans la queue le processus
    insert_ready_tail(prev_head_ready);
}

void init_idle()
{
    const int32_t pid = 0;
    const char name[PROCESS_NAME_MAX] = "idle";
    const State state = STATE_RUNNING;
    process_array[pid] = mem_alloc(sizeof(process));
    process *idle = process_array[pid];
    idle->pid = pid;
    strcpy(idle->name, name);
    idle->state = state;
    idle->execution_stack = mem_alloc(STACK_SIZE * sizeof(int32_t));
    insert_ready_head(idle);
    insert_ready_tail(idle);
}

int32_t create_process(void (*code)(void), char *nom)
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
    process *current_process = process_array[pid];
    current_process->pid = pid;
    strcpy(current_process->name, nom);
    current_process->state = STATE_READY;
    current_process->execution_stack = mem_alloc(STACK_SIZE * sizeof(int32_t));
    current_process->execution_stack[STACK_SIZE - 1] = (int32_t)code;
    current_process->regs[ESP] = (int32_t)&current_process->execution_stack[STACK_SIZE - 1];
    insert_ready_tail(current_process);
    return pid;
}


void ordonnance()
{
    process *next_process = head_ready_process->suiv;
    if (next_process == NULL)
    {
        //si pas d'autre process disponible, alors nous gardons le process actuel
        return;
    }
    process *current_process = head_ready_process;
    insert_ready_head(next_process);
    insert_ready_tail(current_process);
    ctx_sw(current_process->regs, next_process->regs);
}

int32_t running_process_pid()
{
    return head_ready_process->pid;
}

char *running_process_name()
{
    return head_ready_process->name;
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

void proc1(void)
{
    for (;;)
    {
        printf("[%s] pid = %i\n", running_process_name(), running_process_pid());
        for (int32_t i = 0; i < 100 * 1000 * 1000; i++);
        sti();
        hlt();
        cli();
    }
}
