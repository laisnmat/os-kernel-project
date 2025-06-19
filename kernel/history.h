#ifndef HISTORY_H
#define HISTORY_H

#define HISTORY_SIZE 20
#define KBD_BUFFER_SIZE 128

typedef struct
{
    char buffer[HISTORY_SIZE][KBD_BUFFER_SIZE];
    unsigned int cursor_read;
    unsigned int cursor_write;
    unsigned int count;
    unsigned int up_count;
} history_t;

extern history_t history;

void history_add(const char *cmd);
char *history_up(void);
char *history_down(void);
void print_history(void);
void reset_up_count(void);

#endif // HISTORY_H
