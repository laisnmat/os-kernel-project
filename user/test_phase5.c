#include "start.h"
#include <string.h>
#include "syscalls.h"
#include <stdio.h>

int user_test(void *arg)
{
    __printf("Je demarre\n");
    // Exécuter une instruction privilégiée
    __asm__ volatile("hlt"); // Instruction privilégiée qui provoque une exception
    __printf("Je stoppe terminer\n");
    return (int)arg;
}

int user_ES_test(void *arg)
{
    __printf("Je demarre\n");
    // Essayer d'accéder à l'espace d'E/S
    unsigned char rega;
    __asm__ __volatile__("inb %1,%0" : "=a" (rega) : "Nd" (0x3d4));
    __printf("Je stoppe terminer\n");
    return (int)arg;
}

int user_param_test(void *arg)
{
    __printf("Je demarre avec parametre: %s\n", (char *)arg);
    // Le processus se termine
    return (int)arg;
}

int test_phase_5()
{
    __printf("############## Phase 5: Test ##############\n");
    /****
    * Écrire et faire fonctionner le programme suivant :
    *   - imprimer le message Je démarre
    *   - exécuter une instruction privilégiée
    *   - imprimer le message Je stoppe terminer
    * À l'exécution, on doit voir le premier message mais pas le second car lors de
    * l'exécution de l'instruction privilégiée, il doit se produire une exception, que le noyau pourra
    * récupérer pour tuer le programme fautif. Cette gestion des exceptions n'est pas obligatoire
    * dans le cahier de charges de base du projet, et si elle n'est pas implémentée,
    * votre noyau plantera en cas d'exception.
     ****/
    int pid = start(user_test, 512, 200, "UserTest", NULL);
    assert(waitpid(pid, 0) == pid);
    /****
     * C'est une variation du programme précédent : au lieu d'exécuter une instruction privilégiée,
     * le programme essaye d'accéder à l'espace d'E/S, par exemple en tentant de modifier la position
     * du curseur, ou à une zone mémoire protégée, par exemple la mémoire vidéo. Là aussi, le noyau pourra
     * récupérer l'exception et tuer le programme fautif.
     */
    int pid2 = start(user_ES_test, 512, 200, "UserESTest", NULL);
    assert(waitpid(pid2, NULL) == pid2);
    /*
     * Écrire un programme qui crée plusieurs processus en leur passant des paramètres différents.
     * Les processus se contentent d'imprimer les paramètres reçus et se terminent.
     */
    start(user_param_test, 512, 200, "ParamTest", "1");
    start(user_param_test, 512, 200, "ParamTest", "2");
    start(user_param_test, 512, 200, "ParamTest", NULL);
    __printf("3\n");
    start(user_param_test, 512, 200, "ParamTest", "");
    __printf("4");
    __printf("\n############## Fin test phase 5 ##############\n");
    return 1;
}
