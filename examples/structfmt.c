#include "sef/sef.h"
#include <stddef.h>
#include <stdio.h>

typedef struct {
    int x, y;
} Point;
size_t fmt_point(SEF_SinkHandler_t *sink, const Point *p, SEF_KeyVal_t *argv);

int main() {
    SEF_FmtIR_t ir = SEF_IR_BEGIN_REQUIRED("point")
                        SEF_IR_BF(0, 0, NULL)
                        SEF_IR_L("\n")
                      SEF_IR_END();

    static SEF_RegistrySlot_t reg[] = {
        (SEF_RegistrySlot_t){.key="point",.fn=(SEF_FmtFn_t)fmt_point},
        SEF_REGSLOT_TERM
    };
    SEF_Ctx_t ctx = {.registry = reg};
    
    Point p = {6, 7};

    SEF_IPrintf(&ctx, &ir, (const void*[]){&p});
    return 0;
}

size_t fmt_point(SEF_SinkHandler_t *sink, const Point *p, SEF_KeyVal_t *argv) {
    (void)argv;
    int len = snprintf(NULL, 0, "Point(%d, %d)", p->x, p->y);
    if (len < 0) return SEF_Err;
    char buf[len + 1];
    snprintf(buf, len + 1, "Point(%d, %d)", p->x, p->y);
    return SEF_SinkWrite(sink, buf);
}
