#include "process.h"

process process_array[NBPROC];
process *running_process;

void init_idle() {
    const int32_t pid = 0;
    const char name[PROCESS_NAME_LENGTH] = "idle";
    const State state = STATE_RUNNING;
    process *idle = &process_array[pid];
    idle->pid = pid;
    strcpy(idle->name, name);
    idle->state = state;
    running_process = idle;
}

void init_proc1() {
    int32_t pid = 1;
    char name[PROCESS_NAME_LENGTH] = "proc1";
    State state = STATE_READY;
    process *process1 = &process_array[pid];
    process1->pid = pid;
    strcpy(process1->name, name);
    process1->state = state;
    process1->execution_stack[STACK_SIZE-1] = (int32_t) proc1;
    process1->regs[ESP] = (int32_t) &process1->execution_stack[STACK_SIZE-1];
}

void ordonnance() {
    running_process->state = STATE_READY;
    process *next_process = (mon_pid() == NBPROC-1) ? &process_array[0] : &process_array[mon_pid()+1];
    next_process->state = STATE_RUNNING;
    process *current_process = running_process;
    running_process = next_process;
    ctx_sw(current_process->regs, next_process->regs);
}

int32_t mon_pid() {
    return running_process->pid;
}

char *mon_nom() {
    return running_process->name;
}


void idle() {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int32_t i = 0; i < 100000000; i++)
            ;
        ordonnance();
    }
}

void proc1() {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int32_t i = 0; i < 100000000; i++)
            ;
        ordonnance();
    }
}