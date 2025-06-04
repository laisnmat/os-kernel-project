#ifndef PSYS_BASE_CLOCK_H
#define PSYS_BASE_CLOCK_H

#define QUARTZ 0x1234DD
#define CLOCKFREQ 60

#include "display.h"
#include "boot/processor_structs.h"
#include "segment.h"

void tic_PIT(void);

void display_char_top_right(char *chaine);

void init_traitant_IT(int32_t num_IT, void (*traitant)(void));

void masque_IRQ(uint32_t num_IRQ, bool masque);

extern void traitant_IT_32(void);


#endif //PSYS_BASE_CLOCK_H
