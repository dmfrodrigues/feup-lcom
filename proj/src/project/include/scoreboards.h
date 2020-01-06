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
 * @param   p       Pointer to highscores
 * @param   path    Path to file to be written
 */
void (highscores_save)(const highscores_t *p, const char *path);

/**
 * @brief Checks if score is higher than any of the top 3, and if so replaces the highscores.
 * @param   p               Pointer to highscores.
 * @param   score           New score obtained.
 * @param   time_played     Time played
 */
void (check_new_score)(highscores_t *p, int score, int time_played);
/**
 * @brief Updates highscores text.
 * @param p     Pointer to highscores.
 */
void (highscores_update_text)(highscores_t *p);
/**
 * @brief highscore menu state.
 *
 * This function allows to check if the mouse is hovering over the back button, and knowing
 * if it was clicked.
 * @param   menu    Pointer to highscore
 * @param   click   0 if mouse right button is clicked, other value otherwise
 * @return  return 0 if back option is clicked else -1
 */
int (highscore_update_state)(highscores_t *p, int click);
/**
 * @brief Draw highscore on screen buffer.
 * @param   p    Pointer to highscores to be drawn
 */
void (highscores_draw)(highscores_t *p);
/**
 * @brief Destruct menu.
 * @param   p   Pointer to menu to destruct
 */
void (highscores_dtor)(highscores_t *p);

/**
 * @}
 */


#endif /* end of include guard: SCOREBOARD_H_INCLUDED */
