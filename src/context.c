#include "sef/_context.h"
#include "sef/_ds/dynamic_array.h"
#include <stdlib.h>
#include "sef/context.h"

SEF_Ctx_t *SEF_CtxInit() {
    _sefCtx_t *ctx = malloc(sizeof(_sefCtx_t));
    if (!ctx) return NULL;
    int i = _sefDStack_Registry_t_Alloc(&ctx->registry, 10);
    if (1 == i) {
        free(ctx);
        return NULL;
    }
    return (SEF_Ctx_t *)ctx;
}
