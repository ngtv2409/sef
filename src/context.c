/*
TODO: change to BYOM pattern rather than allocation
*/
#include "sef/_pubtypes.h"

void SEF_CtxInit(SEF_Ctx_t *ctx, SEF_RegistrySlot_t *reg) {
    ctx->registry = reg;
}
