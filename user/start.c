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
    if ((int)arg)
        __printf("OK\n");
    //char *msg = "YOUHOU\n";
    //__printf(msg);
    return 0;
}

void user_start(void)
{
    char *msg = "Hello\n";
    __printf(msg);
    start(hello_world, 4050, 228, "User_Proc1", 0);
    char *final_msg = "Exit\n";
    __printf(final_msg);
}
