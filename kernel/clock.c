#include "clock.h"

#include "process.h"
#include "../shared/stdio.h"

unsigned long nb_interruptions = 0;
unsigned long long idt[IDT_ENTRIES];
static unsigned sched_counter = 0;

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
    //char buffer[16];
    //const uint32_t seconds = nb_interruptions / CLOCKFREQ;
    // sprintf(buffer, "%02d:%02d:%02d", seconds / 3600, (seconds / 60) % 60, seconds % 60);
    // display_char_top_right(buffer);
    if (++sched_counter >= CLOCKFREQ / SCHEDFREQ) {
        sched_counter = 0;
        schedule();
    }
}

void init_traitant_IT(unsigned long num_IT, void (*traitant)(void), int dpl)
{
    unsigned long addr = (unsigned long)traitant;
    uint8_t type_attr = 0x8E | (dpl << 5);
    idt[num_IT] = ((uint64_t)(addr & 0xFFFF))
                | ((uint64_t)KERNEL_CS << 16)
                | ((uint64_t)type_attr << 40)
                | ((uint64_t)(addr >> 16) << 48);
}

void masque_IRQ(unsigned long num_IRQ, bool masque)
{
    // val_masq = (val_masq &(~(uint8_t)(1 << num_IRQ))) | (masque << num_IRQ)
    uint8_t val_masq = inb(0x21);
    val_masq = masque
                   ? val_masq | (masque << num_IRQ)
                   : val_masq & (masque << num_IRQ);
    outb(val_masq, 0x21);
}

void clock_settings(unsigned long *quartz, unsigned long *ticks)
{
    *quartz = QUARTZ;
    *ticks = QUARTZ / CLOCKFREQ;
    outb(0x34, 0x43);
    outb(*ticks, 0x40);
    outb(*ticks / 256, 0x40);
}

unsigned long current_clock()
{
    return nb_interruptions;
}
