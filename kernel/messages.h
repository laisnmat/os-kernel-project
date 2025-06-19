#ifndef MESSAGES_H
#define MESSAGES_H

#include <queue.h>

#define MAX_NB_QUEUE 10
#define MAX_QUEUE_CAPACITY (0x3000000 - 0x2800000) // user_stack_heap_end - user_stack_heap

extern unsigned int NB_QUEUE;


/**
 * @brief Obtention d'information sur une file de messages
 *
 * La primitive pcount lit la quantité de données et de processus en attente sur la file fid. Si
 * count n'est pas nul, elle y place une valeur négative égale à l'opposé du nombre de processus
 * bloqués sur file vide ou une valeur positive égale à la somme du nombre de messages dans la file
 * et du nombre de processus bloqués sur file pleine.
 * @param fid Identifiant de la file de messages.
 * @param count  Pointeur vers un entier où sera stocké le nombre de messages et de processus en
 * attente.
 * @return Si la valeur de fid est invalide, pcount signale l'erreur en retournant un code de retour
 * négatif, sinon 0.
 */
int pcount(int fid, int *count);

/**
 * @brief Création d'une file de messages
 *
 * La primitive pcreate alloue une file de capacité égale à la valeur de count. S'il n'y a plus de
 * file disponible ou si la valeur de count est négative ou nulle la valeur de retour de pcreate est
 * strictement négative sinon elle identifie la file qui a été allouée.
 * @param count Capacité de la file de messages à créer. Si count est négatif ou nul, la création
 * échoue.
 * @return Un identifiant de file de messages positif si la création a réussi, ou un code d'erreur
 * négatif si la création a échoué.
 */
int pcreate(int count);

/**
 * @brief Destruction d'une file de messages
 *
 * pdelete détruit la file de messages identifiée par fid et fait passer dans l'état activable, ou
 * actif, tous les processus, s'il en existe, qui se trouvaient bloqués sur la file. Les processus
 * libérés auront une valeur strictement négative comme retour de psend ou preceive. Les messages se
 * trouvant dans la file sont abandonnés.
 *
 * @param fid Identifiant de la file de messages à détruire.
 * @return Si la valeur de fid est invalide, pdelete retourne un code d'erreur négatif, sinon elle
 * retourne 0.
 */
int pdelete(int fid);

/**
 * @brief Réception d'un message sur une file
 *
 * La primitive preceive lit et enlève le premier (plus ancien) message de la file fid. Le message
 * lu est placé dans *message si message n'est pas nul, sinon il est oublié. Lorsque la file était
 * pleine, on la complète immédiatement avec le message du processus bloqué en émission le plus
 * ancien parmi les plus prioritaires, s'il en existe un (processus bloqué). Ce processus devient
 * alors activable ou actif selon sa priorité. L'obtention du premier message de la file peut
 * nécessiter le passage dans l'état bloqué sur file vide jusqu'à ce qu'un autre processus exécute
 * une primitive psend. Il est possible également, qu'après avoir été mis dans l'état bloqué sur
 * file vide, le processus soit remis dans l'état activable par un autre processus ayant exécuté
 * preset ou pdelete. Dans ce cas, la valeur de retour de preceive est strictement négative. Un
 * processus bloqué sur file vide et dont la priorité est changée par chprio, est considéré comme le
 * dernier processus (le plus jeune) de sa nouvelle priorité.
 *
 * @param fid Identifiant de la file de messages.
 * @param message Pointeur vers un entier où sera stocké le message lu. Si message est nul, le
 * message est ignoré.
 * @return Si la valeur de fid est invalide, preceive retourne un code d'erreur négatif, sinon elle
 * retourne 0 si un message a été lu avec succès, ou une valeur strictement négative si le processus
 * a été mis dans l'état activable ou actif sans lire de message.
 */
int preceive(int fid,int *message);

/**
 * @brief Envoi d'un message dans une file
 *
 * La primitive psend envoie le message dans la file identifiée par fid. On distingue trois cas :
 * Si la file est vide et que des processus sont bloqués en attente de message, alors le processus
 * le plus ancien dans la file parmi les plus prioritaires est débloqué et reçoit ce message. Si la
 * file est pleine, le processus appelant passe dans l'état bloqué sur file pleine jusqu'à ce qu'une
 * place soit disponible dans la file pour y mettre le message. Sinon, la file n'est pas pleine et
 * aucun processus n'est bloqué en attente de message. Le message est alors déposé directement dans
 * la file. Il est possible également, qu'après avoir été mis dans l'état bloqué sur file pleine, le
 * processus soit remis dans l'état activable par un autre processus ayant exécuté preset ou
 * pdelete. Dans ce cas, la valeur de retour de psend est strictement négative.
 * @param fid Identifiant de la file de messages.
 * @param message Message à envoyer dans la file. Si message est nul, l'envoi est ignoré.
 * @return Si la valeur de fid est invalide, psend retourne un code d'erreur négatif, sinon elle
 * retourne 0 si le message a été envoyé avec succès, ou une valeur strictement négative si le
 * processus a été mis dans l'état activable ou actif sans envoyer de message.
 */
int psend(int fid, int message);

/**
 * @brief Réinitialisation d'une file de messages
 *
 * La primitive preset vide la file identifiée par la valeur de fid et fait passer dans l'état
 * activable ou actif (selon les priorités) tous les processus, s'il en existe, se trouvant dans
 * l'état bloqué sur file pleine ou dans l'état bloqué sur file vide (ces processus auront une
 * valeur strictement négative comme valeur de retour de psend ou preceive). Les messages se
 * trouvant dans la file sont abandonnés.
 * @param fid Identifiant de la file de messages à réinitialiser.
 * @return Si la valeur de fid est invalide, preset retourne un code d'erreur négatif, sinon elle
 * retourne 0.
 */
int preset(int fid);


#endif //MESSAGES_H
