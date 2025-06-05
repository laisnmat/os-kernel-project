#ifndef PSYS_BASE_CLOCK_H
#define PSYS_BASE_CLOCK_H

#define QUARTZ 0x1234DD
#define CLOCKFREQ 60

#include "display.h"
#include "../shared/stdlib.h"
#include "boot/processor_structs.h"
#include "segment.h"

void tic_PIT(void);

void display_char_top_right(char *chaine);

void init_traitant_IT(int32_t num_IT, void (*traitant)(void));

void masque_IRQ(uint32_t num_IRQ, bool masque);

void clock_settings(uint32_t *quartz, uint32_t *ticks);

uint32_t current_clock();

void wait_clock(uint32_t clock);

extern void traitant_IT_32(void);


#endif //PSYS_BASE_CLOCK_H
