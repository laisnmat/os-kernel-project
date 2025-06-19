#include "syscalls.h"
#include "shell.h"


void user_start(void)
{
    start(start_shell, 256, 3, "sh", 0);
}
