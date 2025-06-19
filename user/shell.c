#include "shell.h"
#include "../kernel/params.h"
#include <stdarg.h>
#include "syscalls.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "lib/lib.h"


typedef struct
{
    const char *name;
    const char *description;
    const void *pt_func;
    const int prio;
    const bool has_args;
} command_t;


int start_test(void *arg)
{
    for (int i = 1; i <= 20; i++)
    {
        test_run(i);
    }
    return (int)arg;
}

int start_test_phase5(void *arg)
{
    test_phase_5();
    return (int)arg;
}

void __printf(const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    cons_write(buf, strlen(buf));
}


int test_args(args_t *arg)
{
    __printf("\033[%dmArguments received:\033[0m\n", secondary_color);
    for (int i = 0; i < arg->argc; i++)
    {
        __printf("\033[%dm\t-Argument %d: \033[0m%s\n", primary_color, i + 1, arg->argv[i]);
    }
    __printf("\033[%dmTotal arguments: \033[0m%d\n", secondary_color, arg->argc);

    return 0;
}

static const command_t commands[] = {
    {"help", "Affiche l'aide", print_help, 0, false},
    {"banner", "Affiche la banniere", print_banner, 0, false},
    {"ps", "Affiche les processus en cours d'execution", ps, 0, false},
    {"exit", "Quitte le shell", exit, 0, false},
    {"quit", "Quitte le shell", exit, 0, false},
    {"test", "Lance les tests du prof", start_test, 128, false},
    {"test_phase5", "Lance les tests de la phase 5", start_test_phase5, 0, false},
    {"clear", "Efface l'ecran", clear, 0, false},
    {"sh", "Lance le shell", start_shell, 0, false},
    {"history", "Affiche l'historique des commandes", print_history, 0, false},
    {"test_args", "Tests du passage d'arguments", test_args, 0, true},
    {"kill", "Tuer un processus", __kill, 0, true},
    {"sleep", "Met le processus courant en attente", __sleep, 0, true},
    {"killall", "Tuer tous les processus du nom passer en arguments", __killall, 0, true},
    {"set", "Change les parametres du systeme", __set, 0, true},
    {"time", "Recupere l'heure actuelle", get_time, 0, false},
};

char *PROMPT = "\033[%dmpsys\033[%dm@\033[%dm2024-2025 \033[%dm- \033[%dmMatthew \033[%dm- \033[%dmJeremy \033[%dm- \033[%dmVincent \n\033[%dm> \033[0m";


void clear()
{
    __printf("\f");
}

void print_banner(void)
{

    __printf("\033[%dm _____                                                    _____   \n", primary_color);
    __printf("\033[%dm( ___ )                                                  ( ___ )  \n", primary_color);
    __printf("\033[%dm |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   |   \n", primary_color);
    __printf("\033[%dm |   |\033[%dm      ____               _      __     ____  _____  \033[%dm|   |   \n", primary_color, secondary_color, primary_color);
    __printf("\033[%dm |   |\033[%dm     / __ \\_________    (_)__  / /_   / __ \\/ ___/  \033[%dm|   |   \n", primary_color, secondary_color, primary_color);
    __printf("\033[%dm |   |\033[%dm    / /_/ / ___/ __ \\  / / _ \\/ __/  / / / /\\__ \\   \033[%dm|   |   \n", primary_color, secondary_color, primary_color);
    __printf("\033[%dm |   |\033[%dm   / ____/ /  / /_/ / / /  __/ /_   / /_/ /___/ /   \033[%dm|   |   \n", primary_color, secondary_color, primary_color);
    __printf("\033[%dm |   |\033[%dm  /_/   /_/   \\____/_/ /\\___/\\__/   \\____//____/    \033[%dm|   |   \n", primary_color, secondary_color, primary_color);
    __printf("\033[%dm |   |\033[%dm                  /___/                             \033[%dm|   |   \n", primary_color, secondary_color, primary_color);
    __printf("\033[%dm |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___|   \n", primary_color);
    __printf("\033[%dm(_____)                                                  (_____)  \n\n", primary_color);
}

void print_prompt(void)
{
    __printf(PROMPT, primary_color, tertiary_color, secondary_color, tertiary_color, primary_color, tertiary_color, primary_color, tertiary_color, primary_color, primary_color);

}

void print_help(void)
{
    for (unsigned int i = 0; i < sizeof(commands) / sizeof(command_t); i++)
    {
        __printf("\033[%dm%s\033[0m - %s\n", secondary_color, commands[i].name, commands[i].description);
    }
}

void get_args(command_parsed_t *args, const char *cmd)
{
    args->args.argc = 0;
    for (int i = 0; i < SHELL_MAX_ARGS; i++)
    {
        args->args.argv[i][0] = '\0'; // Initialisation des arguments
    }
    char *token = strtok((char *)cmd, " ");
    while (token != NULL && args->args.argc < SHELL_MAX_ARGS)
    {
        if (args->executable == NULL)
        {
            args->executable = token; // Le premier token est l'executable
        }
        if (strlen(token) == 0)
        {
            token = strtok(NULL, " ");
            continue; // Ignore les tokens vides
        }
        strncpy(args->args.argv[args->args.argc], token, SHELL_MAX_ARG_LENGTH - 1);
        args->args.argv[args->args.argc][SHELL_MAX_ARG_LENGTH - 1] = '\0'; // Assure la terminaison de la chaîne
        args->args.argc++;
        token = strtok(NULL, " ");
    }
}

int start_shell(void* arg)
{
    print_banner();
    __printf("\033[18mThis is the PSYS shell. Type 'help' for a list of commands.\033[0m\n\n");

    while (1)
    {
        print_prompt();
        char buffer[64];
        int len = cons_read(buffer, 63);
        buffer[len] = '\0';
        command_parsed_t parsed;
        parsed.executable = NULL;
        get_args(&parsed, buffer);
        if (len <= 0)
        {
            continue;
        }
        int found = 0;
        for (unsigned int i = 0; i < sizeof(commands) / sizeof(command_t); i++)
        {
            if (strcmp(parsed.executable, commands[i].name) == 0)
            {
                found = 1;
                if (commands[i].pt_func != NULL)
                {
                    if (commands[i].pt_func == exit)
                    {
                        exit(0); // Quitte le shell
                    }
                    if (commands[i].has_args)
                    {
                        waitpid(start((int (*)(void *))commands[i].pt_func, 256, commands[i].prio ? commands[i].prio : getprio(getpid()) + 1 , commands[i].name, &parsed.args), NULL);

                    }
                    else
                    {
                        waitpid(start((int (*)(void *))commands[i].pt_func, 256, commands[i].prio ? commands[i].prio : getprio(getpid()) + 1 , commands[i].name, 0), NULL);
                    }
                }
                break;
            }
        }
        if (!found)
        {
            __printf("\033[22msh: command not found: %s\033[0m\n", buffer);
        }
    }
    return (int)arg;
}

void get_time(void)
{

    __printf("\033[%dmCurrent time: \033[%dm%d\033[0m tics since boot.\033[0m\n", secondary_color, tertiary_color, current_clock());
}
