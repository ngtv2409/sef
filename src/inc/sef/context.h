#ifndef SEF_CONTEXT_H
#define SEF_CONTEXT_H

typedef struct SEF_Ctx_t SEF_Ctx_t;

#include "sef/registry.h"

/*
 * Create a context from a specifier registry 
 * Should be NULL terminated
 */
SEF_Ctx_t *SEF_CtxCreate(SEF_RegistrySlot_t reg[]);
/*
 * Destroy the context
 * If free_reg is non-zero, a free is performed on the registry
 * Use when the registry is allocated */
void SEF_CtxDestroy(SEF_Ctx_t *ctx, int free_reg);

#endif /* SEF_CONTEXT_H */
