#include "sef/sef.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

size_t fmt_sep(SEF_SinkHandler_t *sink, const char *s, SEF_KeyVal_t *argv);

int main() {
    SEF_FmtIR_t ir = SEF_IR_BEGIN_REQUIRED("sep")
                        SEF_IR_L("Hello, ")
                        SEF_IR_BF(0, 0, NULL)
                        SEF_IR_L("\n")
                      SEF_IR_END();
    static SEF_RegistrySlot_t reg[] = {
        (SEF_RegistrySlot_t){.key="sep",.fn=(SEF_FmtFn_t)fmt_sep},
        SEF_REGSLOT_TERM
    };

    SEF_Ctx_t ctx = {.registry = reg};

    SEF_IPrintf(&ctx, (SEF_FmtIR_t*)&ir, (const void*[]){&"Alice"});
    return 0;
}

size_t fmt_sep(SEF_SinkHandler_t *sink, const char *s, SEF_KeyVal_t *argv) {
    // default
    const char *sep = "-";
    // get arg
    if (argv) {
        SEF_ArgGet(argv, &sep, "sep");
    }

    size_t seplen = strlen(sep);
    size_t slen = strlen(s);
    // final str size
    size_t flen = seplen * (slen - 1) + slen;
    char buf[flen + 1];
    for (size_t i = 0; i < slen; ++i) {
        buf[(seplen + 1) * i] = s[i];
        if (i < slen - 1)
            strncpy(&buf[(seplen + 1) * i + 1], sep, seplen);
    }
    buf[flen] = '\0';
    return SEF_SinkWrite(sink, buf);
}

