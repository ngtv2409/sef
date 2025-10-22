#pragma once

// this module is exported

typedef enum {
/* OK should always be 0 followed directly by other codes */
    SEF_ErrOK = 0,

/* Other codes here */


/* last error, = actual err count
// used in bound checking
// Also correspond to the str Invalid error code in _SEF_ErrStrArray */
    SEF_ErrLast
} SEF_Error_t;

extern const char * const _SEF_ErrStrArray[SEF_ErrLast + 1];

int *_SEF_errno_addr();
#define SEF_Errno (*(_SEF_errno_addr()))
#define SEF_ErrStr(err) ((err >= 0 && err < SEF_ErrLast)? (_SEF_ErrStrArray[err]) : (_SEF_ErrStrArray[SEF_ErrLast]))
