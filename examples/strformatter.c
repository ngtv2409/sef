#include "sef/sef.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// create a toy formatter to use
size_t fmt_upper(SEF_SinkHandler_t *sink, const char *s, SEF_KeyVal_t *argv);

int main() {
    // required upper
    SEF_FmtIR_t ir = SEF_IR_BEGIN_REQUIRED("upper")
                        SEF_IR_L("Hello, ")
                        // use argument at position 0
                        // use format at idx 0 in the required list
                        // NULL argument list
                        SEF_IR_BF(0, 0, NULL)
                        SEF_IR_L("\n")
                      SEF_IR_END();
    // Hello, {0.upper: NULL}

    // define registry
    static SEF_RegistrySlot_t reg[] = {
        (SEF_RegistrySlot_t){.key="upper",.fn=(SEF_FmtFn_t)fmt_upper},
        SEF_REGSLOT_TERM
    };

    // define ctx using reg
    SEF_Ctx_t ctx = {.registry = reg};

    SEF_IPrintf(&ctx, &ir, (const void*[]){&"Alice"});
    // Hello, ALICE
    return 0;
}


size_t fmt_upper(SEF_SinkHandler_t *sink, const char *s, SEF_KeyVal_t *argv) {
    // ignore argv
    (void)argv;

    // Process string
    size_t n = strlen(s);
    char buf[n + 1];
    for (size_t i = 0; i < n; ++i) {
        buf[i] = toupper(s[i]);
    }
    buf[n] = '\0';

    // finallize
    return SEF_SinkWrite(sink, buf);
}
