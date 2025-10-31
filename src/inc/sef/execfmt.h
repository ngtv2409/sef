#pragma once

#include "sef/context.h"
#include "sef/parse.h"
#include <stddef.h>

size_t SEF_IPrintf(SEF_Ctx_t *ctx, SEF_FmtIR_t *ir, void *args[]);
