#include "msg_queue.h"
#include "process.h"

int NB_QUEUE;
msg_queue_t* msg_queue_array[MAX_NB_QUEUE];

void init_msg_queue_array()
{
    NB_QUEUE = 0;
    for (int i = 0; i < MAX_NB_QUEUE; i++)
        msg_queue_array[i] = NULL;
}


msg_queue_t* init_msg_queue(unsigned int fid, unsigned int capacity)
{
    msg_queue_t* queue = mem_alloc(sizeof(msg_queue_t));
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

int add_msg_queue(msg_queue_t* queue, int msg)
{
    if (queue->count >= queue->capacity)
        return -1;

    queue->count++;
    message_t* m = mem_alloc(sizeof(message_t));
    m->message = msg;
    m->fid = queue->fid;
    queue_add(m, &queue->head, message_t, next, fid);
    return queue->count;
}

message_t *pop_msg_queue(msg_queue_t* queue)
{
    if (queue->count == 0)
        return NULL; // Queue is empty

    queue->count--;
    return queue_out(&queue->head, message_t, next);
}

message_t *peek_msg_queue(msg_queue_t* queue)
{
    if (queue->count == 0)
        return NULL; // Queue is empty

    return queue_top(&queue->head, message_t, next);
}

void empty_queue(link* head)
{
    process_t *p = queue_top(head, process_t, next);
    while (p != NULL)
    {
        p->state = STATE_READY;
        p->fid_destroyed_or_reset = true;
        queue_del(p, next);
        queue_add(p, &head_ready, process_t, next, prio);
        p = queue_top(head, process_t, next);
    }
}


void clean_msg_queue(msg_queue_t* queue)
{
    while (!is_empty(queue))
    {
        message_t *m = pop_msg_queue(queue);
        mem_free(m, sizeof(message_t));
    }
}

void reset_queue(msg_queue_t* queue)
{
    queue->count = 0;
    queue->writers_count = 0;
    queue->readers_count = 0;
    empty_queue(&queue->readers_queue);
    empty_queue(&queue->writers_queue);
    clean_msg_queue(queue);
}

void destroy_msg_queue(msg_queue_t* queue)
{
    reset_queue(queue);
    msg_queue_array[queue->fid] = NULL;
    mem_free(queue, sizeof(msg_queue_t));
    NB_QUEUE--;
}

bool is_full(const msg_queue_t* queue)
{
    return queue->count == queue->capacity;
}

bool is_empty(const msg_queue_t* queue)
{
    return queue->count == 0;
}

bool readers_empty(const msg_queue_t* queue)
{
    return queue->readers_count == 0;
}

bool writers_empty(const msg_queue_t* queue)
{
    return queue->writers_count == 0;
}
