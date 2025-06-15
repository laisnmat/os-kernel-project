#include "../shared/string.h"
#include "syscalls.h"
#include "../shared/stdio.h"

int test_run(int n);

int start_test(void *arg)
{
    for (int i = 1; i <= 20; i++)
    {
        printf("\n############## TEST %d ##############\n \n", i);
        test_run(i);
        printf("\n############## FIN TEST %d ##############\n", i);
    }
    return (int)arg;
}


void user_start(void)
{
    start(start_test, 4000, 128, "StartTest", NULL);
}
