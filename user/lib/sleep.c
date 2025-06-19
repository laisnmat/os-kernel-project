#include "../../kernel/params.h"
#include "../../shared/string.h"
#include "../syscalls.h"
#include "lib.h"

static void help()
{
    __printf("\033[%dmUsage:\033[0m sleep \033[%dm <duration>\033[0m\n", secondary_color,
             primary_color);
    __printf("This command sleep the process during <duration> tic.\n");
}

void __sleep(args_t *args)
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

    int duration = atoi(args->argv[1]);
    if (duration <= 0)
    {
        __printf("\033[22mInvalid Duration: %s\033[0m\n", args->argv[1]);
        return;
    }

    wait_clock(duration + current_clock());
}
