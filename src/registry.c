/* 
 > Module Reg
 @ Manages formatter registry
*/

#include "sef/_pubtypes.h"

int strcmp(const char*, const char*);

typedef struct {
    SEF_RegistrySlot_t *registry;
} _sefCtx_t;

int SEF_RegistryExists(SEF_Ctx_t *_ctx, const char *specstr) {
    _sefCtx_t *ctx = (_sefCtx_t*)_ctx;
    SEF_RegistrySlot_t *buf = ctx->registry;
    while (buf->key) {
        if (strcmp(buf->key, specstr) == 0) {
            return 1;
        }
        ++buf;
    }
    return 0;
}
SEF_FmtFn_t SEF_RegistryGet(SEF_Ctx_t *_ctx, const char *specstr) {
    _sefCtx_t *ctx = (_sefCtx_t*)_ctx;
    SEF_RegistrySlot_t *buf = ctx->registry;
    while (buf->key) {
        if (strcmp(buf->key, specstr) == 0) {
            return buf->fn;
        }
        ++buf;
    }
    return NULL;
}
