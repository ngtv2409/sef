#pragma once

/* Errno pattern:
 * Only set in errors, stay the same in success
 * User should reset before potential error if 
 * intended to inspect
 */
// this module is exported

/*
 * Codes are generic and untechnical
 * Must inspect C errno to find deeper details
 */
typedef enum {
/* OK should always be 0 followed directly by other codes */
    SEF_ErrOK = 0,

/* Other codes here */
    /* Memory */
    SEF_ErrMemAllocFailed,

    /* Registry */
    // The function doesn't exist in registry
    SEF_ErrRegFnNotFound,

/* last error, = actual err count
// used in bound checking
// Also correspond to the str Invalid error code in _SEF_ErrStrArray */
    SEF_ErrLast
} SEF_Error_t;

extern const char * const _SEF_ErrStrArray[SEF_ErrLast + 1];

int *_SEF_errno_addr();
#define SEF_Errno (*(_SEF_errno_addr()))
#define SEF_ErrnoClear() (SEF_Errno = SEF_ErrOK)
#define SEF_ErrStr(err) ((err >= 0 && err < SEF_ErrLast)? (_SEF_ErrStrArray[err]) : (_SEF_ErrStrArray[SEF_ErrLast]))
#define SEF_ErrnoStr() (SEF_ErrStr(SEF_Errno))
