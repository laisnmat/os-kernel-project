#include "display.h"

uint32_t LIG_CURSOR;
uint32_t COL_CURSOR;

const params default_params = {BLANC, NOIR, false};
params current_params = default_params;

uint16_t *ptr_mem(uint32_t lig, uint32_t col)
{
    return (uint16_t *)(0xB8000 + 2 * (lig * NB_COLONNE + col));
}

void ecrit_car(uint32_t lig, uint32_t col, char c, params p)
{
    uint16_t *mem = ptr_mem(lig, col);
    *mem = c | (p.cl << 15) | (p.cf << 12) | (p.ct << 8);
}

void place_curseur(uint32_t lig, uint32_t col)
{
    LIG_CURSOR = lig;
    COL_CURSOR = col;
    uint16_t pos = col + lig * NB_COLONNE;
    outb(CURSEUR_BAS, PORT_CURSEUR);
    outb((uint8_t)pos, PORT_DATA);
    outb(CURSEUR_HAUT, PORT_CURSEUR);
    outb((uint8_t)(pos >> 8), PORT_DATA);
}

void efface_ecran(void)
{
    for (uint32_t i = 0; i < NB_LIGNE; i++)
    {
        for (uint32_t j = 0; j < NB_COLONNE; j++)
        {
            ecrit_car(i, j, ' ', default_params);
        }
    }
}

void traite_car(char c)
{
    switch (c)
    {
    case BS:
        if (COL_CURSOR > 0)
            COL_CURSOR--;
        break;
    case HT:
        COL_CURSOR += 8 - COL_CURSOR % 8;
        break;
    case LF:
        LIG_CURSOR++;
        COL_CURSOR = 0;
        break;
    case FF:
        LIG_CURSOR = 0;
        COL_CURSOR = 0;
        efface_ecran();
        break;
    case CR:
        COL_CURSOR = 0;
        break;
    default:
        ecrit_car(LIG_CURSOR, COL_CURSOR, c, current_params);
        COL_CURSOR++;
        break;
    }
    if (LIG_CURSOR + (COL_CURSOR / NB_COLONNE) >= NB_LIGNE && COL_CURSOR % NB_COLONNE == 0)
    {
        defilement();
    }
    place_curseur(LIG_CURSOR, COL_CURSOR);
}

void defilement(void)
{
    for (uint32_t i = 1; i < NB_LIGNE; i++)
    {
        for (uint32_t j = 0; j < NB_COLONNE; j++)
        {
            memmove(ptr_mem(i - 1, j), ptr_mem(i, j), 2);
        }
    }
    for (uint32_t j = 0; j < NB_COLONNE; j++)
    {
        ecrit_car(NB_LIGNE - 1, j, ' ', default_params);
    }
    LIG_CURSOR--;
}

static void parse_pseudo_ansi(const char **str)
{
    const char *s = *str;
    if (*s != '\033' && *s != 27)
        return;
    s++;
    if (*s != '[')
        return;
    s++;
    int code = 0;
    int seen_code = 0;
    while (1)
    {
        code = 0;
        while (*s >= '0' && *s <= '9')
        {
            code = code * 10 + (*s - '0');
            s++;
        }
        // Appliquer le code
        if (code == 0 && seen_code == 0)
        {
            current_params = default_params;
        }
        else if (code == 1)
        {
            current_params.cl = true;
        }
        else if (code >= 10 && code <= 25)
        {
            current_params.ct = code - 10;
        }
        else if (code >= 30 && code <= 45)
        {
            current_params.cf = code - 30;
        }
        seen_code = 1;
        if (*s == ';')
        {
            s++;
        }
        else if (*s == 'm')
        {
            s++;
            break;
        }
        else
        {
            break;
        }
    }
    *str = s - 1;
}

static void affiche_chaine(const char *str, unsigned long size)
{
    for (unsigned long i = 0; i < size;)
    {
        if (str[i] == '\033' || str[i] == 27)
        {
            const char *p = &str[i];
            parse_pseudo_ansi(&p);
            while (str[i] != 'm' && i < size)
                i++;
            i++;
            continue;
        }
        traite_car(str[i]);
        i++;
    }
}

void console_putbytes(char *chaine, int32_t taille)
{
    for (int32_t i = 0; i < taille; i++)
    {
        traite_car(chaine[i]);
    }
}

void cons_write(const char *str, unsigned long size)
{
    if (str == NULL || size <= 0)
        return;
    affiche_chaine(str, size);
}
