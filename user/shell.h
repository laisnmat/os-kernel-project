#ifndef SHELL_H
#define SHELL_H

#define SHELL_MAX_ARGS 16
#define SHELL_MAX_ARG_LENGTH 64

typedef struct
{
    char argv[SHELL_MAX_ARGS][SHELL_MAX_ARG_LENGTH];
    int argc;
} args_t;

typedef struct
{
    char *executable;
    args_t args;
} command_parsed_t;

int test_run(int n);
void test_phase_5(void);

void print_banner(void);
void print_prompt(void);
void print_help(void);
int start_shell(void* arg);
void clear(void);
void __printf(const char *fmt, ...);
void get_time(void);

#endif //SHELL_H
