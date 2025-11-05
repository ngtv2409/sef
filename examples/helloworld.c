#include "sef/sef.h"

int main() {
    // create a context with an empty registry
    SEF_Ctx_t ctx = {(SEF_RegistrySlot_t[]){SEF_REGSLOT_TERM}};

    // create the IR
    SEF_FmtIR_t ir = SEF_IR_BEGIN()
                        SEF_IR_L("Hello, world!\n")
                     SEF_IR_END();

    // use the ir to print
    SEF_IPrintf(&ctx, &ir, (const void *[]){NULL});
    return 0;
}
