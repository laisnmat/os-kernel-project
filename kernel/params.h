#ifndef PARAMS
#define PARAMS 
#include <stdint.h>
#include <string.h>
// Taille de la fenêtre
#define NB_LIGNE 25
#define NB_COLONNE 80


// Couleur
#define NOIR 0
#define BLEU 1
#define VERT 2
#define CYAN 3
#define ROUGE 4
#define MAGENTA 5
#define MARRON 6
#define GRIS 7
#define GRIS_CLAIR 8
#define BLEU_CLAIR 9
#define VERT_CLAIR 10
#define CYAN_CLAIR 11
#define ROUGE_CLAIR 12
#define MAGENTA_CLAIR 13
#define JAUNE 14
#define BLANC 15

// Caractères echappements
#define BS '\b'
#define HT '\t'
#define LF '\n'
#define FF '\f'
#define CR '\r'


// command outb inb
#define CURSEUR_BAS 0x0F
#define CURSEUR_HAUT 0x0E
#define PORT_CURSEUR 0x3D4
#define PORT_DATA 0x3D5

//cursor position
extern uint32_t LIG_CURSOR;
extern uint32_t COL_CURSOR;
#endif