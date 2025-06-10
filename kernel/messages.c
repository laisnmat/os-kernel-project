#include "messages.h"
#include "msg_queue.h"
#include "process.h"

bool is_valid_fid(int fid)
{
    return (fid >= 0 && fid < MAX_NB_QUEUE &&  msg_queue_array[fid] != NULL);
}

int pcreate(int count)
{
    if (NB_QUEUE >= MAX_NB_QUEUE || count <= 0)
        return -1;

    int fid = 0;

    while (fid < MAX_NB_QUEUE && msg_queue_array[fid] != NULL)
        fid++;

    msg_queue_array[fid] = init_msg_queue(fid, count);
    return fid;
}

int pcount(int fid, int *count)
{
    if (!is_valid_fid(fid))
        return -1; // Invalid fid

    if (count != NULL)
    {
        msg_queue * queue = msg_queue_array[fid];
        if (is_empty(queue))
            *count = -queue->readers_count;
        else
            *count = queue->writers_count + queue->count;
    }

    return 0;
}

int preceive(int fid, int *msg)
{
    if (!is_valid_fid(fid))
        return -1; // Invalid fid

    msg_queue *queue = msg_queue_array[fid];

    if (is_empty(queue))
    {
        running_process->state = STATE_BLOCKED_MSG;
        queue_add(running_process, &queue->readers_queue, process, next, prio);
        queue->readers_count++;
        schedule();
    }

    if (running_process->fid_destroyed_or_reset)
    {
        running_process->fid_destroyed_or_reset = false;
        return -2;
    }

    message *m = pop_msg_queue(queue);
    *msg = m->message;

    if (!writers_empty(queue))
    {
        process *writer = queue_out(&queue->writers_queue, process, next);
        writer->state = STATE_READY;
        queue_add(writer, &head_ready, process, next, prio);
        queue->writers_count--;
        schedule();
    }

    return 0;
}


int psend(int fid, int msg)
{
    if (!is_valid_fid(fid))
        return -1; // Invalid fid

    msg_queue *queue = msg_queue_array[fid];

    if (is_full(queue))
    {
        running_process->state = STATE_BLOCKED_MSG;
        queue_add(running_process, &queue->writers_queue, process, next, prio);
        queue->writers_count++;
        schedule();
    }

    if (running_process->fid_destroyed_or_reset)
    {
        running_process->fid_destroyed_or_reset = false;
        return -2;
    }

    add_msg_queue(queue, msg);

    if (!readers_empty(queue))
    {
        process *reader = queue_out(&queue->readers_queue, process, next);
        reader->state = STATE_READY;
        queue_add(reader, &head_ready, process, next, prio);
        queue->readers_count--;
        schedule();
    }

    return 0;
}

int pdelete(int fid)
{
    if (!is_valid_fid(fid))
        return -1; // Invalid fid

    msg_queue *queue = msg_queue_array[fid];

    destroy_msg_queue(queue);
    schedule();
    return 0;
}

int preset(int fid)
{
    if (!is_valid_fid(fid))
        return -1; // Invalid fid

    msg_queue *queue = msg_queue_array[fid];

    reset_queue(queue);
    schedule();
    return 0;
}
