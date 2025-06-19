#include "../../kernel/params.h"
#include "../../shared/string.h"
#include "../syscalls.h"
#include "lib.h"

static void help()
{
    __printf("\033[%dmUsage:\033[0m killall \033[%dm <name>\033[0m\n", secondary_color,
             primary_color);
    __printf("This command terminates all processes with the specified name.\n");
}

void __killall(args_t *args)
{
    if (args->argc < 2)
    {
        __printf("\033[22mUsage: %s <name>\033[0m\n", args->argv[0]);
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

    char *name = args->argv[1];
    __printf("Name: %s\n", name);

    int result = killall(name);
    if (result < 0)
    {
        __printf("\033[22mError: Unable to kill processes with name %s\033[0m\n", name);
    }
    else
    {
        __printf("\033[12m%d Process(es) with name %s has been killed successfully.\033[0m\n", result, name);
    }
}
