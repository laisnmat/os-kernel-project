#include "msg_queue.h"
#include "process.h"

unsigned int NB_QUEUE;
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
    // Vérifie que le mem_alloc ne plante pas
    if (queue == NULL) return NULL;
    queue->messages = mem_alloc(capacity * sizeof(int));
    // Vérifie que le mem_alloc ne plante pas
    if (queue->messages == NULL) return NULL;
    INIT_LIST_HEAD(&queue->readers_queue);
    INIT_LIST_HEAD(&queue->writers_queue);
    queue->capacity = capacity;
    queue->fid = fid;
    queue->count = 0;
    queue->write_position = 0;
    queue->read_position = 0;
    queue->writers_count = 0;
    queue->readers_count = 0;
    NB_QUEUE++;
    return queue;
}

int add_msg_queue(msg_queue_t* queue, int msg)
{
    if (queue->count >= queue->capacity)
        return -1;

    queue->count++;
    queue->messages[queue->write_position++] = msg;
    queue->write_position %= queue->capacity;

    return queue->count;
}

int pop_msg_queue(msg_queue_t* queue)
{
    queue->count--;
    const int msg = queue->messages[queue->read_position++];
    queue->read_position %= queue->capacity;
    return msg;
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
    for (unsigned int i = 0; i < queue->capacity; i++)
    {
        queue->messages[i] = 0; // Reset messages to 0
    }
    queue->write_position = 0;
    queue->read_position = 0;
    queue->count = 0;
}

void reset_queue(msg_queue_t* queue)
{
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
    mem_free(queue->messages, queue->capacity * sizeof(int));
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

bool readers_empty(msg_queue_t* queue)
{
    return queue_empty(&(queue->readers_queue));
}

bool writers_empty(msg_queue_t* queue)
{
    return queue_empty(&(queue->writers_queue));
}
