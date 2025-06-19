#include "history.h"
#include "display.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

history_t history = {.cursor_read = 0, .cursor_write = 0, .count = 0};


static inline unsigned int circ_inc(unsigned int val, unsigned int max)
{
    return (val + 1) % max;
}
static inline unsigned int circ_dec(unsigned int val, unsigned int max)
{
    return (val + max - 1) % max;
}

void __printf(const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    cons_write(buf, strlen(buf));
}

void history_add(const char *cmd)
{
    if (!cmd || strlen(cmd) == 0 || strcmp(cmd, "\r") == 0)
        return;
    strncpy(history.buffer[history.cursor_write], cmd, KBD_BUFFER_SIZE - 1);
    history.buffer[history.cursor_write][KBD_BUFFER_SIZE - 1] = '\0';
    history.cursor_write = circ_inc(history.cursor_write, HISTORY_SIZE);
    if (history.count < HISTORY_SIZE)
        history.count++;
    history.cursor_read = history.cursor_write;
    history.up_count = 0;
}

char *history_up(void)
{
    if (history.count == 0)
        return NULL;
    if (history.up_count >= history.count)
        return history.buffer[history.cursor_read];
    if (history.cursor_read == history.cursor_write)
    {
        history.cursor_read = circ_dec(history.cursor_write, HISTORY_SIZE);
        history.up_count = 1;
        return history.buffer[history.cursor_read];
    }
    history.cursor_read = circ_dec(history.cursor_read, HISTORY_SIZE);
    history.up_count++;
    return history.buffer[history.cursor_read];
}

char *history_down(void)
{
    if (history.count == 0 || history.up_count == 0)
        return NULL;
    history.cursor_read = circ_inc(history.cursor_read, HISTORY_SIZE);
    if (history.cursor_read == history.cursor_write)
    {
        history.up_count = 0;
        return NULL;
    }
    if (strlen(history.buffer[history.cursor_read]) == 0)
        return NULL;
    if (history.up_count > 0)
        history.up_count--;
    return history.buffer[history.cursor_read];
}

void print_history(void)
{
    unsigned int oldest = (history.cursor_write + HISTORY_SIZE - history.count) % HISTORY_SIZE;
    for (unsigned int i = 0; i < history.count; ++i)
    {
        unsigned int idx = (oldest + i) % HISTORY_SIZE;
        if (strlen(history.buffer[idx]) > 0)
            __printf("%d: %s\n", i + 1, history.buffer[idx]);
    }
}

void reset_up_count(void)
{
    history.up_count = 0;
    history.cursor_read = history.cursor_write;
}
