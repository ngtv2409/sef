/*
 > Module Error
 @ Error handling
*/

#ifndef SEF_ERROR_H
#define SEF_ERROR_H

/*
 @ Error code 
*/
typedef enum {
    SEF_ErrOK = 0,

/* Other codes here */

/* Memory */
    SEF_ErrMemAllocFailed,

/* Registry */
    SEF_ErrRegFnNotFound,

/* last error, = actual err count
   used in bound checks
   Also correspond to the str Invalid error code in _SEF_ErrStrArray */
    SEF_ErrLast
} SEF_Error_t;

/* @ Error message array */
extern const char * const _SEF_ErrStrArray[SEF_ErrLast + 1];

/* @ Errno address in memory (thread local) */
extern int *_SEF_errno_addr();
/* @ Macro for transparent errno usage */
#define SEF_Errno (*(_SEF_errno_addr()))
/* @ Sets errno to Ok */
#define SEF_ErrnoClear() (SEF_Errno = SEF_ErrOK)
/* @ Gets the errstr of an error code */
#define SEF_ErrStr(err) ((err >= 0 && err < SEF_ErrLast)? (_SEF_ErrStrArray[err]) : (_SEF_ErrStrArray[SEF_ErrLast]))
/* @ Gets the errstr of current errno */
#define SEF_ErrnoStr() (SEF_ErrStr(SEF_Errno))

#endif /* SEF_ERROR_H */
