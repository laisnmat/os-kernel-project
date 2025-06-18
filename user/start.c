#include "../kernel/params.h"
#include "../shared/stdio.h"
#include "../shared/string.h"
#include "syscalls.h"

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
int test_phase_5();

int start_test(void *arg)
{
    for (int i = 1; i <= 20; i++)
    {
        test_run(i);
    }
    __printf("\033[20m\n############## All tests completed successfully! ##############\n\033[0m");
    return (int)arg;
}

void print_banner(void)
{
    int mainColor = VERT_CLAIR;
    mainColor += 10; //
    int secondaryColor = CYAN_CLAIR;
    secondaryColor += 10;
    __printf("\033[%dm _____                                                    _____   \n", mainColor);
    __printf("\033[%dm( ___ )                                                  ( ___ )  \n", mainColor);
    __printf("\033[%dm |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   |   \n", mainColor);
    __printf("\033[%dm |   |\033[%dm      ____               _      __     ____  _____  \033[%dm|   |   \n", mainColor, secondaryColor, mainColor);
    __printf("\033[%dm |   |\033[%dm     / __ \\_________    (_)__  / /_   / __ \\/ ___/  \033[%dm|   |   \n", mainColor, secondaryColor, mainColor);
    __printf("\033[%dm |   |\033[%dm    / /_/ / ___/ __ \\  / / _ \\/ __/  / / / /\\__ \\   \033[%dm|   |   \n", mainColor, secondaryColor, mainColor);
    __printf("\033[%dm |   |\033[%dm   / ____/ /  / /_/ / / /  __/ /_   / /_/ /___/ /   \033[%dm|   |   \n", mainColor, secondaryColor, mainColor);
    __printf("\033[%dm |   |\033[%dm  /_/   /_/   \\____/_/ /\\___/\\__/   \\____//____/    \033[%dm|   |   \n", mainColor, secondaryColor, mainColor);
    __printf("\033[%dm |   |\033[%dm                  /___/                             \033[%dm|   |   \n", mainColor, secondaryColor, mainColor);
    __printf("\033[%dm |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___|   \n", mainColor);
    __printf("\033[%dm(_____)                                                  (_____)  \n\n", mainColor);
}

int test_cons_read(void *arg)
{

    char *PS1 = "\033[20mpsys\033[12m@\033[13m2024-2025 \033[12m- \033[20mMatthew \033[12m- \033[20mJeremy \033[12m- \033[20mVincent \033[12m \n\033[0m";

    print_banner();

    __printf(PS1);

    while (1)
    {
        __printf("\r\033[12m> \033[0m");
        char buffer[128];
        int len = cons_read(buffer, 127);
        if (strcmp("test", buffer) == 0)
        {
            waitpid(start(start_test, 256, 128, "StartTest", NULL), NULL);
        }
        else if (strcmp("exit", buffer) == 0 || strcmp("quit", buffer) == 0)
        {
            __printf("\033[22mExiting test_cons_read.\033[0m\n");
            break;
        }
        else if (len > 0)
        {
            __printf("%s (length = %d)\n", buffer, len);
        }
    }

    return (int)arg;
}


void user_start(void)
{
    start(test_cons_read, 256, 32, "TestConsRead", NULL);
}
