#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

/**
 * @defgroup    errors  errors
 * @brief Error codes.
 *
 * @{
 */

/** @brief Error Codes */
enum errors {
    SUCCESS = 0,        /** @brief Sucessful */
    NULL_PTR = 250,     /** @brief Null Pointer Error */
    LCF_ERROR,          /** @brief Error originated on LCF */
    SBCR_ERROR,         /** @brief Error on Subscribing Interrupt */
    UNSBCR_ERROR,       /** @brief Error on Unsubscring Interrupt*/
    READ_ERROR,         /** @brief Error on Reading from Port */
    WRITE_ERROR,        /** @brief Error on Writing to Port */
    TIMEOUT_ERROR,      /** @brief Timeout error */
    INVALID_COMMAND,    /** @brief Invalid Command issued */
    INVALID_STATE,      /** @brief State machine reached an invalid state */
    BIOS_CALL_ERROR,    /** @brief Error upon BIOS call */
    OUT_OF_RANGE,       /** @brief Accessing area out of range of memory */
    ALLOC_ERROR,        /** @brief Memory allocation error */
    LOGIC_ERROR,        /** @brief Logic error */
    INVALID_ARG,        /** @brief Invalid argument */
    NOK,                /** @brief Not OK */
    TRANS_REFUSED,      /** @brief Transmission refused */
    TRANS_FAILED,       /** @brief Transmission failed */
    OTHER_ERROR         /** @brief Unspecified error */
};

/**
 * @}
 */

#endif //ERRORS_H_INCLUDED
