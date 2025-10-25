#include "sef/_context.h"
#include "sef/error.h"
#include <stdlib.h>
#include "sef/context.h"

SEF_Ctx_t *SEF_CtxCreate(SEF_RegistrySlot_t reg[]) {
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
