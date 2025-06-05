#include "clock.h"

#include "process.h"
#include "../shared/stdio.h"

uint32_t nb_interruptions = 0;
unsigned long long idt[IDT_ENTRIES];

void display_char_top_right(char *chaine)
{
    for (size_t i = 0; i < strlen(chaine); i++)
    {
        ecrit_car(0, NB_COLONNE - (strlen(chaine) - i), chaine[i], (params){BLANC, NOIR, false});
    }
}

void tic_PIT(void)
{
    outb(0x20, 0x20);
    nb_interruptions++;
    char buffer[16];
    const uint32_t seconds = nb_interruptions / CLOCKFREQ;
    sprintf(buffer, "%02d:%02d:%02d", seconds / 3600, (seconds / 60) % 60, seconds % 60);
    display_char_top_right(buffer);
    ordonnance();
}

void init_traitant_IT(int32_t num_IT, void (*traitant)(void))
{
    uint32_t addr = (uint32_t)traitant;
    idt[num_IT] = ((uint64_t)(addr & 0xFFFF))
                  | ((uint64_t)KERNEL_CS << 16)
                  | ((uint64_t)0x8E00 << 32)
                  | ((uint64_t)(addr >> 16) << 48);
}

void masque_IRQ(uint32_t num_IRQ, bool masque)
{
    // val_masq = (val_masq &(~(uint8_t)(1 << num_IRQ))) | (masque << num_IRQ)
    uint8_t val_masq = inb(0x21);
    val_masq = masque
                   ? val_masq | (masque << num_IRQ)
                   : val_masq & (masque << num_IRQ);
    outb(val_masq, 0x21);
}

void clock_settings(uint32_t *quartz, uint32_t *ticks)
{
    *quartz = QUARTZ;
    *ticks = QUARTZ / CLOCKFREQ;
    outb(0x34, 0x43);
    outb((QUARTZ / CLOCKFREQ) % 256, 0x40);
    outb((QUARTZ / CLOCKFREQ) / 256, 0x40);
}

uint32_t current_clock()
{
    return nb_interruptions;
}

void wait_clock(uint32_t clock)
{
    while (current_clock() < clock)
    {
        hlt();
    }
}
