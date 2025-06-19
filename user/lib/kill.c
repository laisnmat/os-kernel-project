#include "../../kernel/params.h"
#include "../../shared/string.h"
#include "../syscalls.h"
#include "lib.h"

static void help()
{
    __printf("\033[%dmUsage:\033[0m kill \033[%dm <pid>\033[0m\n", secondary_color, primary_color);
    __printf("This command terminates the process with the specified PID.\n");
}

void __kill(args_t *args)
{
    if (args->argc < 2)
    {
        __printf("\033[22mUsage: %s <pid>\033[0m\n", args->argv[0]);
        return;
    }
    for (int i = 1; i < args->argc; i++)
    {
        if (strcmp(args->argv[i], "--help") == 0 || strcmp(args->argv[i], "-h") == 0)
        {
            help();
            return;
        }
    }

    int pid = atoi(args->argv[1]);
    __printf("PID: %d\n", pid);
    if (pid <= 0)
    {
        __printf("\033[22mInvalid PID: %s\033[0m\n", args->argv[1]);
        return;
    }

    int result = kill(pid);
    if (result < 0)
    {
        __printf("\033[22mError: Unable to kill process with PID %d\033[0m\n", pid);
    }
    else
    {
        __printf("\033[12mProcess with PID %d has been killed successfully.\033[0m\n", pid);
    }
}
