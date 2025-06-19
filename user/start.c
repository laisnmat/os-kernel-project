#include "syscalls.h"
#include "shell.h"
#include "../kernel/params.h"

int primary_color = VERT_CLAIR + 10;
int secondary_color = CYAN_CLAIR + 10;
int tertiary_color = VERT + 10;

void user_start(void)
{
    start(start_shell, 2048, 3, "sh", 0);
}
