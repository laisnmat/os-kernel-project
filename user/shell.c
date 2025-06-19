#include "shell.h"
#include "../kernel/params.h"
#include <stdarg.h>
#include "syscalls.h"
#include <stdio.h>
#include <string.h>

typedef struct
{
    const char *name;
    const char *description;
    const void *pt_func;
    const int prio;
} command_t;


int start_test(void *arg)
{
    for (int i = 1; i <= 20; i++)
    {
        test_run(i);
    }
    return (int)arg;
}

static const command_t commands[] = {
    {"help", "Affiche l'aide", print_help, 0},
    {"banner", "Affiche la banniere", print_banner, 0},
    {"ps", "Affiche les processus en cours d'execution", ps, 0},
    {"exit", "Quitte le shell", exit, 0},
    {"quit", "Quitte le shell", exit, 0},
    {"echo", "Active ou desactive l'affichage des entrees clavier", cons_echo, 0},
    {"test", "Lance les tests", start_test, 128},
    {"clear", "Efface l'ecran", clear, 0},
    {"sh", "Lance le shell", start_shell, 0},
    {"history", "Affiche l'historique des commandes", print_history, 0},
};

char *PROMPT = "\033[%dmpsys\033[%dm@\033[%dm2024-2025 \033[%dm- \033[%dmMatthew \033[%dm- \033[%dmJeremy \033[%dm- \033[%dmVincent \n\033[%dm> \033[0m";


void __printf(const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    cons_write(buf, strlen(buf));
}

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


int start_shell(void* arg)
{
    print_banner();

    while (1)
    {
        print_prompt();
        char buffer[64];
        int len = cons_read(buffer, 63);
        buffer[len] = '\0';
        if (len <= 0)
        {
            continue; 
        }
        int found = 0;
        for (unsigned int i = 0; i < sizeof(commands) / sizeof(command_t); i++)
        {
            if (strcmp(buffer, commands[i].name) == 0)
            {
                found = 1;
                if (commands[i].pt_func != NULL)
                {
                    if (commands[i].pt_func == exit)
                    {
                        exit(0); // Quitte le shell
                    }
                    waitpid(start((int (*)(void *))commands[i].pt_func, 256, commands[i].prio ? commands[i].prio : getprio(getpid()) + 1 , commands[i].name, 0), NULL); // Appel de la fonction
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
