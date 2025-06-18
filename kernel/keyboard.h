#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_data(char *str);

/**
 * @brief Configuration du terminal
 *
 * @param on Si on est nul, désactive l'écho sur la console, sinon le réactive.
 */
void cons_echo(int on);

/**
 * @brief Lecture sur le terminal
 * Si length est nul, cette fonction retourne 0. Sinon, elle attend que l'utilisateur ait tapé une
 * ligne complète terminée par le caractère 13 puis transfère dans le tableau string soit la ligne
 * entière (caractère 13 non compris), si sa longueur est strictement inférieure à length, soit les
 * length premiers caractères de la ligne. Finalement, la fonction retourne à l'appelant le nombre
 * de caractères effectivement transmis. Les caractères frappés et non prélevés restent dans le
 * tampon associé au clavier et seront prélevés aux appels suivants. Le caractère de fin de ligne
 * (13) n'est jamais transmis à l'appelant.
 *
 * Lorsque length est exactement égal au nombre de caractères frappés, fin de ligne non comprise, le
 * marqueur de fin de ligne reste dans le tampon. Le prochain appel récupèrera une ligne vide.
 * @param string Pointeur vers le tableau de caractères où la ligne lue sera stockée.
 * @param length Longueur maximale désirée pour la lecture. Si 0, la fonction retourne immédiatement
 * 0.
 * @return
 */
int cons_read(char *string, unsigned long length);

#endif
