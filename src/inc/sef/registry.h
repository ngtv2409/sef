#pragma once

/* Format function arguments
 * Key-Value string pair */
typedef struct {
     char *key, *value;
} SEF_FmtFnArg_t;

typedef char *(*SEF_FmtFn_t)(const char *, SEF_FmtFnArg_t *);

typedef struct {
    const char *key;
    SEF_FmtFn_t fn;
} SEF_RegistrySlot_t;

#include "sef/context.h"

int SEF_RegistryExists(SEF_Ctx_t *ctx, const char *specstr);
SEF_FmtFn_t SEF_RegistryGet(SEF_Ctx_t *ctx, const char *specstr);
