/*
TODO: change to BYOM pattern rather than allocation
*/
#include "sef/sef.h"

void SEF_CtxInit(SEF_Ctx_t *ctx, SEF_RegistrySlot_t *reg) {
    ctx->registry = reg;
}
