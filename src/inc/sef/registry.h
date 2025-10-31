#pragma once
#include "sef/writer.h"
#include <stddef.h>

/* Format function arguments
 * @_sefArg_t */
typedef struct SEF_FmtFnArg_t SEF_FmtFnArg_t;

typedef size_t (*SEF_FmtFn_t)(SEF_SinkHandler *, const void *, SEF_FmtFnArg_t *);

typedef struct {
    const char *key;
    SEF_FmtFn_t fn;
} SEF_RegistrySlot_t;
#define SEF_RegistryTERMINATOR ((SEF_RegistrySlot_t){NULL, NULL})

#include "sef/context.h"

int SEF_RegistryExists(SEF_Ctx_t *ctx, const char *specstr);
SEF_FmtFn_t SEF_RegistryGet(SEF_Ctx_t *ctx, const char *specstr);
