#ifndef ZOMBIES_H_INCLUDED
#define ZOMBIES_H_INCLUDED

/**
 * @defgroup    zombies    zombies
 * @ingroup proj
 * @ingroup singleplayer
 * @brief   Zombies mode.
 *
 * @{
 */

#include "scoreboards.h"

/**
* @brief Zombies.
* @return  SUCCESS if operation was successful, other value otherwise
*/
int (zombies)(highscores_t *zombies_highscore);

/**
 * @brief Zombies ranking.
 * @param zombies_highscore Highscores.
 */
void (zombies_ranking)(highscores_t *zombie_highscore);

/**
 * @}
 */

#endif //ZOMBIES_H_INCLUDED
