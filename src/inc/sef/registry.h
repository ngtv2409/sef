#ifndef SEF_REGISTRY_H
#define SEF_REGISTRY_H
#include "sef/args.h"
#include "sef/writer.h"
#include <stddef.h>

typedef size_t (*SEF_FmtFn_t)(SEF_SinkHandler *, const void *, const SEF_KeyVal_t *);

typedef struct {
    const char *key;
    SEF_FmtFn_t fn;
} SEF_RegistrySlot_t;
#define SEF_RegistryTERMINATOR ((SEF_RegistrySlot_t){NULL, NULL})

#include "sef/context.h"

int SEF_RegistryExists(SEF_Ctx_t *ctx, const char *specstr);
SEF_FmtFn_t SEF_RegistryGet(SEF_Ctx_t *ctx, const char *specstr);

#endif /* SEF_REGISTRY_H */
