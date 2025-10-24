#include "sef/error.h"

#ifdef _MSC_VER
    #define THREAD_LOCAL __declspec(thread)
#else
    #define THREAD_LOCAL __thread
#endif

static THREAD_LOCAL int _sefErrno = SEF_ErrOK;
const char * const _SEF_ErrStrArray[SEF_ErrLast + 1] = {
    // ErrOK
    "Ok",

    // ErrLast
    "Unknown error code"
};

int *_SEF_errno_addr() {
    return &_sefErrno;
}
