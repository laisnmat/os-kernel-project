#include "../shared/string.h"
#include "syscalls.h"
#include "../shared/stdio.h"

void __printf(const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    cons_write(buf, strlen(buf));
}

int hello_world(void *arg)
{
    wait_clock(300);
    __printf("YOUHOU\n");
    return (int)arg;
}

void user_start(void)
{
    __printf("You are in a user process!\n");
    int pid = start(hello_world, 4000, 128, "HelloWorld", 0);
    int retval = 0;
    waitpid(pid, &retval);
    __printf("Process %d finished with return value: %d\n", pid, retval);

    __printf("Exiting user process...\n");
}
