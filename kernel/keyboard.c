#include "display.h"
#include "keyboard.h"
#include "process.h"

#include <stdbool.h>
#include <stddef.h>

#define KBD_BUFFER_SIZE 128

typedef struct {
    char buffer[KBD_BUFFER_SIZE];
    unsigned int count;
    unsigned int cursor;
    unsigned int head;
    bool echo_enabled;
} kbd_buffer_t;

LIST_HEAD(blocked_io_processes);

static kbd_buffer_t kbd_buffer = {
    .count = 0,
    .cursor = 0,
    .head = 0,
    .echo_enabled = true
};


int kbd_buffer_push(char c)
{
   if (kbd_buffer.count >= KBD_BUFFER_SIZE)
   {
       return -1;
   }
    kbd_buffer.count++;
    kbd_buffer.buffer[kbd_buffer.cursor] = c;
    kbd_buffer.cursor = (kbd_buffer.cursor + 1) % KBD_BUFFER_SIZE;
    if (kbd_buffer.echo_enabled) traite_car(c);
    return kbd_buffer.count;
}

int kbd_buffer_pop(char *c)
{
    if (kbd_buffer.count == 0) return -1;
    if (c != NULL)
        *c = kbd_buffer.buffer[kbd_buffer.head];
    kbd_buffer.count--;
    kbd_buffer.head = (kbd_buffer.head + 1) % KBD_BUFFER_SIZE;
    return kbd_buffer.count;
}


int kbd_buffer_backspace(void)
{
    if (kbd_buffer.count == 0 || kbd_buffer.buffer[kbd_buffer.cursor] == CR || kbd_buffer.buffer[kbd_buffer.cursor] == HT) return -1;
    kbd_buffer.count--;
    kbd_buffer.cursor = (kbd_buffer.cursor - 1 + KBD_BUFFER_SIZE) % KBD_BUFFER_SIZE;
    if (kbd_buffer.echo_enabled) cons_write("\b \b", 3);
    return kbd_buffer.buffer[kbd_buffer.cursor];
}



void cons_echo(int on)
{
    kbd_buffer.echo_enabled = on != 0;
}

void keyboard_data(char *str)
{
    for (char c = *str; c != '\0'; c = *(++str)) {
        switch (c)
        {
        case BS:
        case 127:
            kbd_buffer_backspace();
            break;
        case CR:
            cons_write("\n", 1);
            kbd_buffer_push(c);
            if (!queue_empty(&blocked_io_processes))
            {
                process_t *p = queue_out(&blocked_io_processes, process_t, next);
                p->state = STATE_READY;
                queue_add(p, &head_ready, process_t, next, prio);
                schedule();
            }
            break;
        case '\002':
            if (kbd_buffer.count == 0 || kbd_buffer.buffer[kbd_buffer.cursor] == CR || kbd_buffer.buffer[kbd_buffer.cursor] == HT) break;
            kbd_buffer.cursor = (kbd_buffer.cursor - 1 + KBD_BUFFER_SIZE) % KBD_BUFFER_SIZE;
            kbd_buffer.count--;
            if (kbd_buffer.echo_enabled) traite_car('\b');
            break;
        default:
            kbd_buffer_push(c);
            break;
        }

    }

}

int cons_read(char *string, unsigned long length)
{
    if (length == 0)
        return 0;
    char c;
    unsigned long size;
    for (size = 0; size < length; size++)
    {
        if (kbd_buffer.count == 0)
        {
            running_process->state = STATE_BLOCKED_IO;
            queue_add(running_process, &blocked_io_processes, process_t, next, prio);
            schedule();
        }
        kbd_buffer_pop(&c);
        if (c == CR)
            break;
        string[size] = c;

    }

    string[size] = '\0';

    return size;
}
