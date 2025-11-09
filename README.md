# SEF
**SEF**, shorts for **Simple Extensible Formatter**, is a simple C format 
backend. It allows users to create custom functions as formatter and 
use them in a simple unified interface.

Version 1.0.0 (stable)

Note: The documentation is still unfinished/half-formed

## Features
**SEF** allows you to format any object with custom functions following core 
principles called *Formatter* in a composable way, here is an example:
```C 
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
```
A formatter is simply a function with the signature
```C
size_t f(SEF_SinkHandler_t *sink, const void *s, SEF_KeyVal_t *argv);
```
Which returns the number of characters written to the sink, usually done by 
simply return `SEF_SinkWrite(sink, buf)`. Formatters can be connected together 
via the intermidiate representation declarative macros.

**SEF** also allows the formatter to be struct (or just anything) 
since it receives a generic pointer `void *`, meaning you can do this:
```C 
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
```

Upcoming features include:
- Heap version for less constrained formatting (SEF is currently fully stack with 256 bytes constraint as well as max chain and max dep per ir)
- Ctx Union, using multiple registry at the same time. Which allows `import`ation of formatters.

## Installation
You can easily build it from source
```sh
git clone https://github.com/ngtv2409/sef
cd sef 
mkdir build
cd build
cmake ..
make
```
It will generates two files for shared and static targets.
The header is located at `src/inc/sef/sef.h`
