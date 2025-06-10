#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include "mem.h"
#include "messages.h"
#include "stdbool.h"


typedef struct msg_queue
{
    link head;                    // Queue to hold messages
    unsigned int capacity;        // Maximum number of messages in the queue
    unsigned int count;           // Current number of messages in the queue
    unsigned int fid;             // Unique identifier for the message queue
    link readers_queue;      // Queue to hold processes waiting to receive messages
    link writers_queue;      // Queue to hold processes waiting to send messages
    unsigned int writers_count; // Number of processes waiting to receive messages
    unsigned int readers_count;    // Number of processes waiting to send messages
} msg_queue;

extern msg_queue* msg_queue_array[MAX_NB_QUEUE];

void init_msg_queue_array();

msg_queue* init_msg_queue(unsigned int fid ,unsigned int capacity);

int add_msg_queue(msg_queue* queue, int message);

message *pop_msg_queue(msg_queue* queue);

message *peek_msg_queue(msg_queue* queue);

void reset_queue(msg_queue* queue);

void destroy_msg_queue(msg_queue* queue);

bool is_full(const msg_queue* queue);

bool is_empty(const msg_queue* queue);

bool readers_empty(const msg_queue* queue);

bool writers_empty(const msg_queue* queue);

#endif //MSG_QUEUE_H
