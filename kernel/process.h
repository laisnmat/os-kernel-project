#ifndef PROCESSUS_H
#define PROCESSUS_H
#include "../shared/stdint.h"
#include "stddef.h"
#include "string.h"
#include "cpu.h"
#include "stdio.h"
#include "mem.h"
#define STACK_SIZE 256
#define NUMBER_REGISTERS 5
#define PROCESS_NAME_MAX 16
#define NBPROC 8
#define EBX 0
#define ESP 1
#define EBP 2
#define ESI 3
#define EDI 4

typedef enum State
{
    STATE_RUNNING,
    STATE_READY,
    STATE_BLOCKED_MSG,
    STATE_BLOCKED_SEM,
    STATE_BLOCKED_IO,
    STATE_WAIT_CHILD,
    STATE_SLEEPING,
    STATE_ZOMBIE
} State;

typedef struct process
{
    int32_t pid;
    char name[PROCESS_NAME_MAX];
    State state;
    int32_t regs[NUMBER_REGISTERS];
    int32_t *execution_stack;
    struct process *suiv;
    uint32_t time_before_wake_up;
} process;

extern process *process_array[NBPROC];
extern void ctx_sw(int32_t *prev_regs, int32_t *next_regs);
void init_idle();
void init_proc1();
void idle();
void proc1();
void ordonnance();
int32_t running_process_pid();
char *running_process_name();
int32_t create_process(void (*code)(void), char *nom);
void sleep(uint32_t seconds);

#endif //PROCESSUS_H
