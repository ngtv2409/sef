**SEF** is trivial to use. After building it and locate the header by 
following the instructions in **README**. You can start using it knowing these principles:

1. **Formatter**s are functions with the signature `size_t (SEF_SinkHandler_t *, const void *, SEF_KeyVal_t *)`, 
which returns the number of chars written to the sink, done by `SEF_SinkWrite`. You can do anything in the 
formatters but you should avoid side effects because it creates inconsistency between dry-run and the next print.
2. **Ctx** is the environment of the execution, it contains the *registry*, which is a pool of formatters and their keys.
3. **IR** is stateless, it does not know anything about the environment. All it does is defining the execution steps.

With these in mind, you can make sense of this example
```c 
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
```
(You can find this file in `examples/strformatter.c`)
`fmt_upper` is just a simple idiomatic formatter, it processes the string and make every letters uppercase. 
Then writes to the sink by a call to `SEF_SinkWrite`.

Then we create an IR to represent the format
```c
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
```
It requires `upper`, then adds a node literal `"Hello, "`, next it calls the formatter at index 0 to the argument at position 0 
with a NULL `argv`.

To actually use, we do the following:
```c 
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
```
Which simply defines a registry (available formatters) and a context. Then call the print functions. 
Depsite the long setup, it is very intuitive and only have to be done once. We should get the result
```
Hello, ALICE
```
in `stdout`.
