#include "common.h"



int atoi(const char *str) {
    int result = 0;
    int sign = 1;

    // Ignorer les espaces initiaux
    while (*str == ' ' || *str == '\t') {
        str++;
    }

    // Gérer les signes
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    // Conversion caractère par caractère
    while (*str != '\0') {
        if (*str <= '9' && *str >= '0')
        {
            result = result * 10 + (*str - '0');
            str++;
        }
        else
        {
            return -1;
        }
    }

    return sign * result;
}
