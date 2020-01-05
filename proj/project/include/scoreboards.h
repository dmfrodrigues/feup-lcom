#ifndef SCOREBOARD_H_INCLUDED
#define SCOREBOARD_H_INCLUDED

/**
 * @defgroup scoreboards scoreboards
 * @ingroup proj
 * @brief Scoreboards module.
 *
 * @{
 */

#include "libs.h"
#include <stdint.h>

/**
 * @brief Score.
 */
typedef struct  score_info score_info_t;

/**
 * @brief Table of highscores.
 */
typedef struct highscores highscores_t;
/**
 * @brief Construct score.
 * @param   score           Game score.
 * @param   time_played     Time elapsed on game.
 * @return      Pointer to constructed score, or NULL if failed.
 */
score_info_t* (score_ctor)(int score, int time_played);

/**
 * @brief Destruct score.
 * @param   p   Pointer to score to destruct
 */
void (score_dtor)(score_info_t *p);

/**
 * @brief Construct highscores.
 * @param   fnt     Font to use when rendering highscores text
 * @param   path    Path to file to be read with highscores values.
 * @return      Pointer to constructed highscores, or NULL if failed.
 */
highscores_t* (highscores_ctor)(const font_t *fnt, const char *path);
/**
 * @brief Save highscore into file.
 * @param f     Path to file to be written
 */
void (highscores_save)(const highscores_t *p, const char *path);
/**
 * @brief Destruct menu.
 * @param   p   Pointer to menu to destruct
 */
void (highscores_dtor)(highscores_t *p);

/**
 * @}
 */


#endif /* end of include guard: SCOREBOARD_H_INCLUDED */
