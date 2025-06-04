#include "display.h"

uint32_t LIG_CURSOR;
uint32_t COL_CURSOR;

uint16_t *ptr_mem(uint32_t lig, uint32_t col){
    return (uint16_t *)(0xB8000 + 2 * (lig * NB_COLONNE + col));
}

void ecrit_car(uint32_t lig, uint32_t col, char c, params p) {
    uint16_t *mem = ptr_mem(lig, col);
    *mem = c | (p.cl << 15) | (p.cf << 12) | (p.ct << 8);
}

void place_curseur(uint32_t lig, uint32_t col) {
    LIG_CURSOR = lig;
    COL_CURSOR = col;
    uint16_t pos = col + lig * NB_COLONNE;
    outb(CURSEUR_BAS, PORT_CURSEUR);
    outb((uint8_t)pos, PORT_DATA);
    outb(CURSEUR_HAUT, PORT_CURSEUR);
    outb((uint8_t)(pos >> 8), PORT_DATA);
}

void efface_ecran(void){
    for (uint32_t i = 0; i < NB_LIGNE; i++){
        for (uint32_t j = 0; j < NB_COLONNE; j++){
            ecrit_car(i, j, ' ', (params){NOIR, NOIR, false});
        }
    }
}

void traite_car(char c){
    switch (c)
    {
        case BS:
            if (COL_CURSOR > 0) COL_CURSOR--;
            break;
        case HT:
            COL_CURSOR += 8 - COL_CURSOR%8;
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
            ecrit_car(LIG_CURSOR, COL_CURSOR, c, (params){BLANC, NOIR, false});
            COL_CURSOR++;
            break;
    }
    place_curseur(LIG_CURSOR, COL_CURSOR);
}

void defilement(void){
    for(uint32_t i = 1; i < NB_LIGNE; i++){
        for(uint32_t j = 0; j < NB_COLONNE; j++){
            memmove(ptr_mem(i-1, j), ptr_mem(i, j), 2);
        }
    }
    for(uint32_t j = 0; j < NB_COLONNE; j++){
        ecrit_car(NB_LIGNE-1, j, ' ', (params){BLANC, BLANC, false});
    }
}

void console_putbytes(char *chaine, int32_t taille){
    for (int32_t i = 0; i < taille; i++){
        traite_car(chaine[i]);
    }
}