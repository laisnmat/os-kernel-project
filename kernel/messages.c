#include "messages.h"
#include "msg_queue.h"
#include "process.h"

bool is_valid_fid(int fid)
{
    return (fid >= 0 && fid < MAX_NB_QUEUE &&  msg_queue_array[fid] != NULL);
}

int pcreate(int count)
{
    if (NB_QUEUE >= MAX_NB_QUEUE || count <= 0 || count >= 1073741827)
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
        msg_queue_t * queue = msg_queue_array[fid];
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

    msg_queue_t *queue = msg_queue_array[fid];

    if (is_empty(queue))
    {
        running_process->state = STATE_BLOCKED_MSG;
        queue_add(running_process, &queue->readers_queue, process_t, next, prio);
        running_process->head_waiting_queue = &queue->readers_queue;
        queue->readers_count++;
        schedule();
        if (running_process->fid_destroyed_or_reset)
        {
            running_process->fid_destroyed_or_reset = false;
            return -2;
        }
        *msg = running_process->buffered_msg;
        return 0;
    }

    if (running_process->fid_destroyed_or_reset)
    {
        running_process->fid_destroyed_or_reset = false;
        return -2;
    }

    message_t *m = pop_msg_queue(queue);
    *msg = m->message;

    //ne pas lâcher dans la nature les processus
    if (!writers_empty(queue))
    {
        process_t *writer = queue_out(&queue->writers_queue, process_t, next);
        add_msg_queue(queue, writer->buffered_msg);
        writer->state = STATE_READY;
        queue_add(writer, &head_ready, process_t, next, prio);
        queue->writers_count--;
        schedule();
    }

    return 0;
}


int psend(int fid, int msg)
{
    if (!is_valid_fid(fid))
        return -1; // Invalid fid

    msg_queue_t *queue = msg_queue_array[fid];

    if (is_full(queue))
    {
        running_process->state = STATE_BLOCKED_MSG;
        running_process->buffered_msg=msg;
        queue_add(running_process, &queue->writers_queue, process_t, next, prio);
        running_process->head_waiting_queue = &queue->writers_queue;
        queue->writers_count++;

        schedule();
        int ret = running_process->fid_destroyed_or_reset ? -2 : 0;
        running_process->fid_destroyed_or_reset = false;
        return ret;
    }

    if (running_process->fid_destroyed_or_reset)
    {
        running_process->fid_destroyed_or_reset = false;
        return -2;
    }



    if (is_empty(queue) && !readers_empty(queue))
    {
        process_t *reader = queue_out(&queue->readers_queue, process_t, next);
        reader->state = STATE_READY;
        reader->buffered_msg=msg;
        queue_add(reader, &head_ready, process_t, next, prio);
        queue->readers_count--;
        schedule();
        return 0;
    }

    add_msg_queue(queue, msg);

    return 0;
}

int pdelete(int fid)
{
    if (!is_valid_fid(fid))
        return -1; // Invalid fid

    msg_queue_t *queue = msg_queue_array[fid];

    destroy_msg_queue(queue);
    schedule();
    return 0;
}

int preset(int fid)
{
    if (!is_valid_fid(fid))
        return -1; // Invalid fid

    msg_queue_t *queue = msg_queue_array[fid];

    reset_queue(queue);
    schedule();
    return 0;
}
