#ifndef SEF_EXECFMT_H
#define SEF_EXECFMT_H

#include "sef/context.h"
#include "sef/parse.h"
#include <stddef.h>

size_t SEF_IPrintf(SEF_Ctx_t *ctx, SEF_FmtIR_t *ir, void *args[]);

#endif /* SEF_EXECFMT_H */
