#include "process.h"
#include "string.h"
#include "mem.h"
#include "../shared/queue.h"
process *process_array[NBPROC];
process *running_process = NULL;
LIST_HEAD(head_ready);
LIST_HEAD(head_sleeping);
LIST_HEAD(head_zombie);
#define MAX(x, y) (((x)>(y))?(x):(y))

void set_zombie(process *p)
{
    State prev_state = p->state;
    p->state = STATE_ZOMBIE;
    if (prev_state != STATE_RUNNING)
    {
        queue_del(p, next);
    }
    queue_push_head(p, &head_zombie, process, next);
}

void destroy(process *p_to_destroy)
{
    if (p_to_destroy == running_process) {
        set_zombie(p_to_destroy);
        return;
    }
    if (p_to_destroy->state != STATE_RUNNING)
    {
        queue_del(p_to_destroy, next);
    }
    else
    {
        running_process = NULL;
    }
    if (p_to_destroy->parent != NULL)
    {
        queue_del(p_to_destroy, children.next);
    }
    int pid = p_to_destroy->pid;
    mem_free(p_to_destroy->execution_stack, p_to_destroy->ssize * sizeof(int32_t));
    mem_free(p_to_destroy, sizeof(process));
    process_array[pid] = NULL;
}

void destroy_children(process *parent)
{
    process *child = queue_entry(parent->children.head.next, process, children.next);
    while (&child->children.next != &parent->children.head)
    {
        process *next_child = queue_entry(child->children.next.next, process, children.next);

        if (child->parent != NULL)
        {
            queue_del(child, children.next);
            child->parent = NULL;
        }
        if (child->state == STATE_ZOMBIE)
        {
            destroy(child);
        }
        child = next_child;
    }
}

void terminate(process *p)
{
    if (p->parent != NULL && p->parent->state == STATE_WAIT_CHILD && (
            p->parent->wait_pid == p->pid || p->parent->wait_pid < 0))
    {
        p->parent->state = STATE_READY;
        p->parent->wait_pid = p->pid;
        queue_add(p->parent, &head_ready, process, next, prio);
    }
    destroy_children(p);
    if (p->parent == NULL)
    {
        destroy(p);
    }
    else
    {
        set_zombie(p);
    }
    schedule();
}

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
    INIT_LIST_HEAD(&idle->children.head);
    idle->parent = NULL;
    running_process = idle;
}

void exit(int retval)
{
    running_process->retval = retval;
    terminate(running_process);
    while (1)
    {
    }
}

int kill(int pid)
{
    // verifier que le processus appelant est bien le parrent du proc à tuer
    if (pid <= 0 || pid >= NBPROC || process_array[pid] == NULL || process_array[pid]->state ==
        STATE_ZOMBIE)
    {
        return -1; // Processus n'existe pas
    }
    process *target_process = process_array[pid];
    target_process->retval = 0;
    terminate(target_process);
    return 0;
}

void process_wrapper(void)
{
    /** Execution stack layout:
    *  +----------------------+
    *  |         arg          | ssize - 1
    *  +----------------------+
    *  *       pt_func        | ssize - 2
    *  +----------------------+
    **/
    unsigned long ssize = running_process->ssize;
    int (*pt_func)(void *) = (int (*)(void *))running_process->execution_stack[ssize - 2];
    void *arg = (void *)running_process->execution_stack[ssize - 1];
    int retval = pt_func(arg);
    exit(retval);
}

int waitpid(int pid, int *retvalp)
{
    bool wait_any_child = (pid < 0);
    // Si le processus appelant n'a pas de fils
    if (queue_empty(&running_process->children.head))
        return -1;

    // Pid négatif, on attend n'importe quel fils
    if (wait_any_child)
    {
        // On vérifie qu'il y a pas un fils qui aurait déjà fini
        process *next_child = queue_entry(running_process->children.head.next, process,
                                          children.next);
        while (&next_child->children.next != &running_process->children.head)
        {
            if (next_child->state == STATE_ZOMBIE)
            {
                if (retvalp != NULL)
                {
                    *retvalp = next_child->retval;
                }
                int child_pid = next_child->pid;
                destroy(next_child);
                return child_pid;
            }
            next_child = queue_entry(next_child->children.next.next, process, children.next);
        }
    }
    /*
      * Si pid invalide
      * Si le processus n'est pas un fils
    */
    if (pid >= NBPROC || (!wait_any_child && process_array[pid]->parent != running_process))
        return -1;

    running_process->wait_pid = pid;
    //si on n'a pas d'enfant zombie en pid -1 / l'enfant qu'on attend n'est pas un zombie
    //alors on attend qu'il fini
    if (wait_any_child || process_array[pid]->state != STATE_ZOMBIE)
    {
        running_process->state = STATE_WAIT_CHILD;
        schedule();
    }
    process *child_process = process_array[running_process->wait_pid];
    if (retvalp != NULL)
    {
        *retvalp = child_process->retval;
    }
    pid = child_process->pid;
    destroy(child_process);
    return pid;
}


int start(int (*pt_func)(void *), unsigned long ssize, int prio, const char *name, void *arg)
{
    ssize = MAX(ssize, MIN_STACK_SIZE);
    if (ssize >= MAX_STACK_SIZE)
    {
        return -1;
    }
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
    current_p->ssize = ssize;
    current_p->execution_stack = mem_alloc(ssize * sizeof(int32_t));
    /** Execution stack layout:
    *  +----------------------+
    *  |         arg          | ssize - 1
    *  +----------------------+
    *  *      pt_func         | ssize - 2
    *  +----------------------+
    *  | call process_wrapper | ssize - 3
    *  +----------------------+ <-- %esp
    **/
    current_p->execution_stack[ssize - 1] = (int32_t)arg;
    current_p->execution_stack[ssize - 2] = (int32_t)pt_func;
    current_p->execution_stack[ssize - 3] = (int32_t)process_wrapper;
    current_p->regs[ESP] = (int32_t)&current_p->execution_stack[ssize - 3];
    current_p->prio = prio;
    INIT_LIST_HEAD(&current_p->children.head);
    current_p->parent = running_process;
    queue_push_head(current_p, &(current_p->parent->children.head), process, children.next);
    queue_add(current_p, &head_ready, process, next, prio);
    schedule();
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
    if (pid < 0 || pid >= NBPROC || process_array[pid] == NULL || newprio <= 0 || newprio > MAXPRIO
        || process_array[pid]->state == STATE_ZOMBIE)
    {
        return -1;
    }
    process *p = process_array[pid];
    int oldprio = p->prio;
    p->prio = newprio;
    // Repositionner le processus dans la file d'attente si nécessaire
    if (p->state == STATE_READY)
    {
        queue_del(p, next);
        queue_add(p, &head_ready, process, next, prio);
    }
    schedule();
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
