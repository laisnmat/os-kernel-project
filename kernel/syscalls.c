#include "clock.h"
#include "debug.h"
#include "display.h"
#include "keyboard.h"
#include "mem.h"
#include "messages.h"
#include "process.h"
#include "history.h"
#include <stdint.h>

typedef struct
{
    uint32_t eax; // numéro de syscall
    uint32_t ebx; // arg1
    uint32_t ecx; // arg2
    uint32_t edx; // arg3
    uint32_t esi; // arg4
    uint32_t edi; // arg5
} syscall_args_t;

int syscalls_handler_c(syscall_args_t *args)
{
    int ret = 0;
    switch (args->eax)
    {
    case 1:
        exit(args->ebx);
        break;
    case 2:
        if ((char *)args->ebx < (char *)0x1000000)
            ret = -1;
        else
            cons_write((const char *)args->ebx, args->ecx);
        break;
    case 3:
        if ((char *)args->esi < (char *)0x1000000 && args->esi != 0)
            ret = -1;
        else
            ret = start((int (*)(void *))args->ebx, args->ecx, args->edx, (const char *)args->esi,
                        (void *)args->edi);
        break;
    case 4:
        if ((int *)args->ecx < (int *)0x1000000 && args->ecx != 0)
            ret = -1;
        else
            ret = waitpid(args->ebx, (int *)args->ecx);
        break;
    case 5:
        ret = kill(args->ebx);
        break;
    case 6:
        ret = chprio(args->ebx, args->ecx);
        break;
    case 7:
        ret = getprio(args->ebx);
        break;
    case 8:
        ret = getpid();
        break;
    case 9:
        ret = pcreate(args->ebx);
        break;
    case 10:
        ret = pdelete(args->ebx);
        break;
    case 11:
        if ((int *)args->ecx < (int *)0x1000000 && args->ecx != 0)
            ret = -1;
        else
            ret = pcount(args->ebx, (int *)args->ecx);
        break;
    case 12:
        ret = psend(args->ebx, args->ecx);
        break;
    case 13:
        if ((int *)args->ecx < (int *)0x1000000 && args->ecx != 0)
            ret = -1;
        else
            ret = preceive(args->ebx, (int *)args->ecx);
        break;
    case 14:
        ret = preset(args->ebx);
        break;
    case 15:
        wait_clock(args->ebx);
        break;
    case 16:
        if (((int *)args->ecx < (int *)0x1000000) || ((int *)args->ebx < (int *)0x1000000))
            ret = -1;
        else
            clock_settings((unsigned long *)args->ebx, (unsigned long *)args->ecx);
        break;
    case 17:
        ret = current_clock();
        break;
    case 18:
        cons_echo(args->ebx);
        break;
    case 19:
        if ((char *)args->ebx < (char *)0x1000000)
            ret = -1;
        else
            ret = cons_read((char *)args->ebx, args->ecx);
        break;
    case 20:
        ps();
        break;
    case 21:
        print_history();
        break;
    case 22:
        ret = killall((char *)args->ebx);
        break;
    default:
        break;
    }
    return ret;
}
