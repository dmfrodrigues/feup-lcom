#ifndef XMP_UTILS_H_INCLUDED
#define XMP_UTILS_H_INCLUDED

/**
 * @defgroup xpm_utils xpm_utils
 * @ingroup utils
 * @brief XPM utilities module.
 *
 * @{
 */

/**
 * @brief Save XPM array of strings as a XPM2 text file.
 * @param   p   XPM to be saved
 * @param   s   Path to save the XPM as XPM2 file
 */
void xpm_save_as_xpm2(const char* const* p, const char *s);
/**
 * @brief Load XPM2 file as a XPM array of strings.
 * @param   s   Path to XPM2 file to be loaded
 * @return      Array of strings representing the loaded XPM
 */
char** xpm_load_xpm2(const char *s);

/**
 * @}
 */

#endif //XMP_UTILS_H_INCLUDED
