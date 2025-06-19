#include "display.h"
#include "keyboard.h"
#include "process.h"
#include "history.h"

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
    if (kbd_buffer.echo_enabled && c >= 32) traite_car(c);
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
    char removed_c = kbd_buffer.buffer[kbd_buffer.cursor];
    if (kbd_buffer.echo_enabled) {
        if (removed_c < 32) {
            // Effacement des caractères de contrôle (double pour effacer le ^ en +)
            cons_write("\b \b\b \b", 6);
        } else {
            // Effacement des caractères lambda
            cons_write("\b \b", 3);
        }
    }
    return kbd_buffer.buffer[kbd_buffer.cursor];
}



void cons_echo(int on)
{
    kbd_buffer.echo_enabled = on != 0;
}

void keyboard_data(char *str)
{
    if (strcmp(str, "\033[A") == 0) //up arrow
    {
        char *cmd = history_up();
        while (kbd_buffer_backspace() >= 0); // Clear the buffer
        if (cmd != NULL)
        {
            for (char c = *cmd; c != '\0'; c = *(++cmd)) {
                kbd_buffer_push(c);
            }
        }
        return;
    }
    if (strcmp(str, "\033[B") == 0) //down arrow
    {
        char *cmd = history_down();
        while (kbd_buffer_backspace() >= 0); // Clear the buffer
        if (cmd != NULL)
        {
            for (char c = *cmd; c != '\0'; c = *(++cmd)) {
                kbd_buffer_push(c);
            }
        }
        return;
    }
    reset_up_count();
    if (strcmp(str, "\033[C") == 0 || strcmp(str, "\033[D") == 0)
    {
        // Ignore left/right arrow keys
        return;
    }
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
            if (c < 32) {
                if (kbd_buffer.echo_enabled) {
                    char ctrl_str[2] = {'^', c+'@'};
                    cons_write(ctrl_str, 2);
                }
                kbd_buffer_push(c);
                break;
            }
            if (c <= 126) {
                kbd_buffer_push(c);
            }
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
        string[size] = c;
        if (c == CR)
            break;

    }

    string[size] = '\0';
    history_add(string);
    return size;
}
