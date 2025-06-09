#ifndef PSYS_BASE_CLOCK_H
#define PSYS_BASE_CLOCK_H

#define QUARTZ 0x1234DD
#define CLOCKFREQ 100
#define SCHEDFREQ 50

#include "display.h"
#include "scheduler.h"
#include "../shared/stdlib.h"
#include "boot/processor_structs.h"
#include "segment.h"

void tic_PIT(void);

void display_char_top_right(char *chaine);

void init_traitant_IT(unsigned long num_IT, void (*traitant)(void));

void masque_IRQ(unsigned long num_IRQ, bool masque);

void clock_settings(unsigned long *quartz, unsigned long *ticks);

unsigned long current_clock();

void wait_clock(unsigned long clock);

extern void traitant_IT_32(void);


#endif //PSYS_BASE_CLOCK_H
