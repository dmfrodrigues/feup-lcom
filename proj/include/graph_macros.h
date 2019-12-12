#ifndef GRAPHICS_MACROS_H_INCLUDED
#define GRAPHICS_MACROS_H_INCLUDED

#define VC_BIOS_SERV  0x10 /** @brief Interrupt service of video card */
#define VBE_CALL      0x4F /** @brief VBE call function specifier */

#define MBYTE_BASE  0x0         /** @brief Base address (zero address) */
#define MBYTE_SIZE  0xFFFFF     /** @brief Size of a mebibyte */

// Graphics Functions
#define VBE_CTRL_INFO       0x00    /** @brief Get VBE Controller Information */
#define VBE_MD_INFO         0x01    /** @brief Get VBE Mode Information */
#define SET_VBE_MD          0x02    /** @brief Set VBE Mode */

// Error codes (AH)
#define AH_SUCCESS          0x00    /** @brief Success code on BIOS call */
#define AH_FUNC_CALL_FAIL   0x01    /** @brief Function call failed */
#define AH_FUNC_NOT_SUPP    0x02    /** @brief Function call is not supported in current HW configuration */
#define AH_FUNC_INVALID     0x03    /** @brief Invalid function in current video mode */

// Graphics modes
#define INDEXED_1024_768        0x105
#define DIRECT_640_480_888      0x110
#define DIRECT_800_600_888      0x115
#define DIRECT_1024_768_888     0x118
#define DIRECT_1280_1024_565    0x11A
#define DIRECT_1280_1024_888    0x11B
#define LINEAR_FRAME_BUFFER_MD  BIT(14)

// Colors in RBG (8 bit)
#define BLACK               0x000000
#define WHITE               0xFFFFFF

#endif /* end of include guard: GRAPHICS_MACROS_H_INCLUDED */
