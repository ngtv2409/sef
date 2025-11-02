/*
TODO: change to BYOM pattern rather than allocation
*/
#include "sef/error.h"
#include "sef/sef.h"

#include <stdlib.h>


typedef struct {
    SEF_RegistrySlot_t *registry;
} _sefCtx_t;

SEF_Ctx_t *SEF_CtxCreate(SEF_RegistrySlot_t *reg) {
    _sefCtx_t *ctx = malloc(sizeof(_sefCtx_t));
    if (!ctx) {
        SEF_Errno = SEF_ErrMemAllocFailed;
        return NULL;
    }
    ctx->registry = reg;
    return (SEF_Ctx_t *)ctx;
}


void SEF_CtxDestroy(SEF_Ctx_t *_ctx, int free_reg) {
    _sefCtx_t *ctx = (_sefCtx_t *)_ctx;
    if (free_reg) {
        free(ctx->registry);
    }
    free(ctx);
}
