#include "msg_queue.h"
#include "process.h"

int NB_QUEUE;
msg_queue* msg_queue_array[MAX_NB_QUEUE];

void init_msg_queue_array()
{
    NB_QUEUE = 0;
    for (int i = 0; i < MAX_NB_QUEUE; i++)
        msg_queue_array[i] = NULL;
}


msg_queue* init_msg_queue(unsigned int fid, unsigned int capacity)
{
    msg_queue* queue = mem_alloc(sizeof(msg_queue));
    INIT_LIST_HEAD(&queue->head);
    INIT_LIST_HEAD(&queue->readers_queue);
    INIT_LIST_HEAD(&queue->writers_queue);
    queue->capacity = capacity;
    queue->fid = fid;
    queue->count = 0;
    queue->writers_count = 0;
    queue->readers_count = 0;
    return queue;
}

int add_msg_queue(msg_queue* queue, int msg)
{
    if (queue->count >= queue->capacity)
        return -1;

    queue->count++;
    message* m = mem_alloc(sizeof(message));
    m->message = msg;
    m->fid = queue->fid;
    queue_add(m, &queue->head, message, next, fid);
    return queue->count;
}

message *pop_msg_queue(msg_queue* queue)
{
    if (queue->count == 0)
        return NULL; // Queue is empty

    queue->count--;
    return queue_out(&queue->head, message, next);
}

message *peek_msg_queue(msg_queue* queue)
{
    if (queue->count == 0)
        return NULL; // Queue is empty

    return queue_top(&queue->head, message, next);
}

void empty_queue(link* head)
{
    process *p = queue_top(head, process, next);
    while (p != NULL)
    {
        p->state = STATE_READY;
        p->fid_destroyed_or_reset = true;
        queue_del(p, next);
        queue_add(p, &head_ready, process, next, prio);
        p = queue_top(head, process, next);
    }
}


void clean_msg_queue(msg_queue* queue)
{
    while (!is_empty(queue))
    {
        message *m = pop_msg_queue(queue);
        mem_free(m, sizeof(message));
    }
}

void reset_queue(msg_queue* queue)
{
    queue->count = 0;
    queue->writers_count = 0;
    queue->readers_count = 0;
    empty_queue(&queue->readers_queue);
    empty_queue(&queue->writers_queue);
    clean_msg_queue(queue);
}

void destroy_msg_queue(msg_queue* queue)
{
    reset_queue(queue);
    msg_queue_array[queue->fid] = NULL;
    mem_free(queue, sizeof(msg_queue));
    NB_QUEUE--;
}

bool is_full(const msg_queue* queue)
{
    return queue->count == queue->capacity;
}

bool is_empty(const msg_queue* queue)
{
    return queue->count == 0;
}

bool readers_empty(const msg_queue* queue)
{
    return queue->readers_count == 0;
}

bool writers_empty(const msg_queue* queue)
{
    return queue->writers_count == 0;
}
