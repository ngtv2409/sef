/*
TODO: change to BYOM pattern rather than allocation
*/
#include "sef/sef.h"

typedef struct {
    SEF_RegistrySlot_t *registry;
} _sefCtx_t;

void SEF_CtxInit(SEF_Ctx_t *_ctx, SEF_RegistrySlot_t *reg) {
    _sefCtx_t *ctx = (_sefCtx_t*)_ctx;
    ctx->registry = reg;
}
