#ifndef PROCESSUS_H
#define PROCESSUS_H

#include "clock.h"
#include "stddef.h"
#include "string.h"
#include "cpu.h"
#include "stdio.h"
#include "mem.h"
#include "../shared/debug.h"
#include "../shared/stdint.h"
#include "../shared/queue.h"

#define MIN_STACK_SIZE 64
#define MAX_STACK_SIZE 4096

#define NUMBER_REGISTERS 5
#define PROCESS_NAME_MAX 16
#define NBPROC 8
#define EBX 0
#define ESP 1
#define EBP 2
#define ESI 3
#define EDI 4
#define MAXPRIO 256

#define queue_add_by_time(ptr_elem, head, type, listfield, wakeuptime) \
    do {                                                               \
        link *__cur_link=head;                                         \
        type *__elem = (ptr_elem);                                     \
        link *__elem_link=&((__elem)->listfield);                      \
        assert((__elem_link->prev == 0) && (__elem_link->next == 0));  \
    do  __cur_link=__cur_link->next;                                   \
    while ( (__cur_link != head) &&                                    \
    (((queue_entry(__cur_link,type,listfield))->wakeuptime)            \
    > ((__elem)->wakeuptime)) );                                       \
    __elem_link->next=__cur_link;                                      \
    __elem_link->prev=__cur_link->prev;                                \
    __cur_link->prev->next=__elem_link;                                \
    __cur_link->prev=__elem_link;                                      \
} while (0)

#define queue_push_head(ptr_elem, head, type, listfield)               \
    do {                                                               \
        link *__head_link = (head);                                    \
        type *__elem      = (type *)(ptr_elem);                        \
        link *__elem_link = &(__elem->listfield);                      \
        assert(__elem_link->prev == NULL && __elem_link->next == NULL);\
        __elem_link->next           = __head_link;                     \
        __elem_link->prev           = __head_link->prev;               \
        __head_link->prev->next     = __elem_link;                     \
        __head_link->prev           = __elem_link;                     \
    } while (0)


typedef enum State
{
    STATE_RUNNING,
    STATE_READY,
    STATE_BLOCKED_MSG,
    STATE_BLOCKED_SEM,
    STATE_BLOCKED_IO,
    STATE_WAIT_CHILD,
    STATE_SLEEPING,
    STATE_ZOMBIE
} State;


typedef struct process
{
    int32_t pid;
    int16_t prio;
    char name[PROCESS_NAME_MAX];
    State state;
    int32_t regs[NUMBER_REGISTERS];
    int32_t *execution_stack;
    unsigned long ssize;
    link next;
    unsigned long wakeup_time;
    int retval;
    struct process *parent;

    struct
    {
        link head;
        link next;
    } children;

    int wait_pid;
} process;


void destroy(process *p_to_destroy);
extern process *process_array[NBPROC];
extern void ctx_sw(int32_t *prev_regs, int32_t *next_regs);
extern process *running_process;
extern process *process_self_killed;
extern link head_ready;
extern link head_sleeping;
extern link head_zombie;
// Les primitives de gestions des processus

/**
 * @brief Initialisation du processus idle
 *
 * Le processus idle est le premier processus à être lancé par le kernel.
 * Il est toujours en état de fonctionnement et ne se termine jamais.
 */
void start_idle();
/**
 * Démarrage d'un nouveau processus
 * @param pt_func Pointeur sur l'adresse du programme (de la fonction) à lancer dans le processus.
 * @param ssize Nombre d'octets utlisables sans compter l'espace nécessaire au stockage de l'adresse de retour, du paramètre etc... Libérée lors de la terminaison d'un processus
 * @param prio Priorité du processus. La priorité est un nombre compris entre 1 et MAXPRIO.
 * @param name Nom du processus. Doit être une chaîne de caractères de taille inférieure à PROCESS_NAME_MAX.
 * @param arg Argument passé à la fonction pt_func. Peut être NULL.
 */
int start(int (*pt_func)(void *), unsigned long ssize, int prio, const char *name, void *arg);


/**
 * @brief Terminaison du processus en cours.
 * @param retval Valeur de retour du processus. Peut être NULL.
 */
void exit(int retval);

/**
 * @brief Terminaison forcée d'un processus
 *
 * La primitive kill termine le processus identifié par la valeur pid. Si ce processus était bloqué dans une file d'attente pour un quelconque élément système, alors il en est retiré.
 * @param pid Identifiant du processus à terminer.
 * @return 0 si le processus a été tué, -1 si le processus n'existe pas ou si l'appelant n'est pas le processus parent.
 */

int kill(int pid);

/**
 * @brief Attente et récupération de la valeur de retour d'un fils terminé
 *
 * Si le paramètre pid est négatif, le processus appelant attend qu'un de ses fils, n'importe lequel, soit terminé et récupère (le cas échéant) sa valeur de retour dans *retvalp, à moins que retvalp soit nul. Cette fonction renvoie une valeur strictement négative si aucun fils n'existe ou sinon le pid de celui dont elle aura récupéré la valeur de retour.
 *
 * Pour un fils qui a été terminé par kill, la valeur de retour (retvalp) est nulle.
 *
 * Si le paramètre pid est positif, le processus appelant attend que son fils ayant ce pid soit terminé ou tué et récupère sa valeur de retour dans *retvalp, à moins que retvalp soit nul. Cette fonction échoue et renvoie une valeur strictement négative s'il n'existe pas de processus avec ce pid ou si ce n'est pas un fils du processus appelant. En cas de succès, elle retourne la valeur pid.
 *
 * Le processus fils terminé n'est définitivement détruit, et enlevé de la liste des fils que lorsque sa valeur de retour a été récupérée, ou que son père est lui-même terminé et ne pourra donc jamais récupérer cette valeur.
 *
 * @param pid
 * @param retvalp
 * @return
 */
int waitpid(int pid, int *retvalp);

/**
 * @brief Obtention de la priorité d'un processus
 *
 * Le processus fils terminé n'est définitivement détruit, et enlevé de la liste des fils que lorsque sa valeur de retour a été récupérée, ou que son père est lui-même terminé et ne pourra donc jamais récupérer cette valeur.
 *
 * @param pid Identifiant du processus dont on veut connaître la priorité.
 * @return
 */
int getprio(int pid);

/**
 * @brief Changement de priorité d'un processus
 *
 * La primitive chprio donne la priorité newprio au processus identifié par la valeur de pid. Si la priorité du processus change et qu'il était en attente dans une file, il doit y être replacé selon sa nouvelle priorité.
 * Si la valeur de newprio ou de pid est invalide, la valeur de retour de chprio est strictement négative, sinon elle est égale à l'ancienne priorité du processus pid.
 * @param pid
 * @param newprio
 * @return
 */
int chprio(int pid, int newprio);

/**
 * @brief Identification du processus appelant
 *
 * @return La valeur de retour de getpid est le pid du processus appelant cette primitive.
 */
int getpid(void);


/**
 * @brief Attente temporisée
 *
 * Passe le processus dans l'état endormi jusqu'à ce que le nombre d'interruptions horloge depuis le démarrage du noyau, passé en paramètre, soit atteint ou dépassé.
 *
 * @param clock Nombre d'interruptions horloge à attendre avant de réveiller le processus.
 */
void wait_clock(unsigned long clock);


#endif //PROCESSUS_H
