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

int test_run(int n);

int start_test(void *arg)
{
    for (int i = 1; i <= 20; i++)
    {
        __printf("\n############## TEST %d ##############\n \n", i);
        test_run(i);
        __printf("\n############## FIN TEST %d ##############\n", i);
    }
    return (int)arg;
}


void user_start(void)
{
    start(start_test, 1000000, 128, "StartTest", NULL);
}
