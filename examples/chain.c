#include "sef/sef.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

size_t fmt_upper(SEF_SinkHandler_t *sink, const char *s, SEF_KeyVal_t *argv);
size_t fmt_undprefix(SEF_SinkHandler_t *sink, const char *s, SEF_KeyVal_t *argv);

int main() {
    SEF_FmtIR_t ir = SEF_IR_BEGIN_REQUIRED("upper", "unpre")
                        SEF_IR_L("Hello, ")
                        SEF_IR_BF(0, 0, NULL)
                            // a pipe formatter (fmtidx, argv)
                            SEF_IR_PF(1, NULL)
                        SEF_IR_L("\n")
                      SEF_IR_END();
    static SEF_RegistrySlot_t reg[] = {
        (SEF_RegistrySlot_t){.key="upper",.fn=(SEF_FmtFn_t)fmt_upper},
        (SEF_RegistrySlot_t){.key="unpre",.fn=(SEF_FmtFn_t)fmt_undprefix},
        SEF_REGSLOT_TERM
    };

    SEF_Ctx_t ctx = {.registry = reg};

    SEF_IPrintf(&ctx, (SEF_FmtIR_t*)&ir, (const void*[]){&"Alice"});
    return 0;
}

size_t fmt_upper(SEF_SinkHandler_t *sink, const char *s, SEF_KeyVal_t *argv) {
    (void)argv;
    size_t n = strlen(s);
    char buf[n + 1];
    for (size_t i = 0; i < n; ++i) {
        buf[i] = toupper(s[i]);
    }
    buf[n] = '\0';
    // a finiallize step of any fmt
    return SEF_SinkWrite(sink, buf);
}
size_t fmt_undprefix(SEF_SinkHandler_t *sink, const char *s, SEF_KeyVal_t *argv) {
    (void)argv;
    size_t n = strlen(s);
    char buf[n + 1 + 1];
    buf[0] = '_';
    for (size_t i = 0; i < n; ++i) {
        buf[i + 1] = s[i];
    }
    buf[n + 1] = '\0';
    return SEF_SinkWrite(sink, buf);
}

