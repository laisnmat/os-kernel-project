#include "process.h"

process *process_array[NBPROC];
process *head_process_activable;
process *tail_process_activable;

void insert_head_activable(process *new_process) {
    process *prev_head = head_process_activable;
    head_process_activable = new_process;
    if (prev_head != NULL)
        prev_head->state = STATE_READY;
    new_process->state = STATE_RUNNING;
    //new_process->suiv = prev_head; pour l'instant on ne veut pas que l'ancienne tete soit le suivant mais plutot à la queue
}

void insert_tail_activable(process *new_process) {
    process *prev_tail = tail_process_activable;
    tail_process_activable = new_process;
    if (prev_tail != NULL)
        prev_tail->suiv = new_process;
    new_process->suiv = NULL;
    new_process->state = STATE_READY;
}

void init_idle() {
    const int32_t pid = 0;
    const char name[PROCESS_NAME_MAX] = "idle";
    const State state = STATE_RUNNING;
    process_array[pid] = mem_alloc(sizeof(process));
    process *idle = process_array[pid];
    idle->pid = pid;
    strcpy(idle->name, name);
    idle->state = state;
    idle->execution_stack = mem_alloc(STACK_SIZE*sizeof(int32_t));
    insert_head_activable(idle);
    insert_tail_activable(idle);
}

int32_t cree_processus(void (*code)(void), char *nom) {
    uint32_t pid = 0;
    while (pid < NBPROC && process_array[pid] != NULL) {
        pid++;
    }
    if (pid >= NBPROC) {
        return -1;
    }
    process_array[pid] = mem_alloc(sizeof(process));
    process *current_process = process_array[pid];
    current_process->pid = pid;
    strcpy(current_process->name, nom);
    current_process->state = STATE_READY;
    current_process->execution_stack = mem_alloc(STACK_SIZE*sizeof(int32_t));
    current_process->execution_stack[STACK_SIZE-1] = (int32_t) code;
    current_process->regs[ESP] = (int32_t) &current_process->execution_stack[STACK_SIZE-1];
    insert_tail_activable(current_process);
    return pid;
}


void ordonnance() {
    process *next_process = head_process_activable->suiv;
    if (next_process == NULL) { //si pas d'autre process disponible, alors nous gardons le process actuel
        return;
    }
    process *current_process = head_process_activable;
    insert_head_activable(next_process);
    insert_tail_activable(current_process);
    ctx_sw(current_process->regs, next_process->regs);
}

int32_t mon_pid() {
    return head_process_activable->pid;
}

char *mon_nom() {
    return head_process_activable->name;
}

void idle(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int32_t i = 0; i < 100 * 1000 * 1000; i++)
            ;
        sti();
        hlt();
        cli();
    }
}

void proc1(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int32_t i = 0; i < 100 * 1000 * 1000; i++)
            ;
        sti();
        hlt();
        cli();
    }
}