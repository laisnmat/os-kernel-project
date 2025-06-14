#ifndef PSYS_BASE_DISPLAY_H
#define PSYS_BASE_DISPLAY_H
#include "cpu.h"
#include "params.h"
#include "stdbool.h"

typedef struct params
{
    uint8_t ct;
    uint8_t cf;
    bool cl;
} params;

uint16_t *ptr_mem(uint32_t lig, uint32_t col);

void ecrit_car(uint32_t lig, uint32_t col, char c, params p);

void place_curseur(uint32_t lig, uint32_t col);

void efface_ecran(void);

void traite_car(char c);

void defilement(void);

void cons_write(const char *str, unsigned long size);

#endif //PSYS_BASE_DISPLAY_H
