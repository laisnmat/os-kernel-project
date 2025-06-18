#include "process.h"

#include <stdio.h>

int gpf_handler()
{
    printf("\nGeneral Protection Fault Catch \n");
    printf("Process fautif : %s (pid %d)\n",
           running_process->name, getpid()
        );
    kill(getpid());
    while (1)
    {
    }
}
