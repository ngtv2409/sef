/*
SEF - Simple Extensible Formatter

A library providing a formatting backend with simple core principles.
Extendable via user-defined functions

See the docs directory for more details
*/

#ifndef SEF_H
#define SEF_H

// for FILE
#include <stdio.h>
// for NULL
#include <stdlib.h> // IWYU pragma: keep
// for size_t
#include <stddef.h>

/*ma 
For size_t returning functions, max is reserved for errors
*/
#define SEF_Err ((size_t)~0)

/* Typedefs */
/*tp opaque
Sink abstraction for write destinations
*/
typedef struct SEF_SinkHandler_t SEF_SinkHandler_t;

/*tp
Key-value pair
*/
typedef struct {
    const char *key, *val;
} SEF_KeyVal_t;
#define SEF_KEYVAL_TERM ((SEF_KeyVal_t){NULL, NULL})

/*tp 
Format function signature
*/
typedef size_t (*SEF_FmtFn_t)(SEF_SinkHandler_t *, const void *, const SEF_KeyVal_t *);

/*tp
Slot structure in registry
*/
typedef struct {
    const char *key;
    SEF_FmtFn_t fn;
} SEF_RegistrySlot_t;
#define SEF_REGSLOT_TERM ((SEF_RegistrySlot_t){NULL, NULL})

/*tp 
Context of format execution.
*/
typedef struct {
    const SEF_RegistrySlot_t *registry;
} SEF_Ctx_t;

/*tp
Intermidiate Representation node type enumerator
*/
typedef enum {
    _SEFNODE_NULL = 0, _SEFNODE_LTR , _SEFNODE_BFMT, _SEFNODE_PFMT
} _sefNodeType_t;

/*tp
Intermidiate Representation node type
*/
typedef struct {
    int type;
    union {
        struct {
            const char *str;
        } ltr;
        struct {
            size_t pos;
            // this id is the order in deplist
            size_t fmtid;
            SEF_KeyVal_t *argv;
        } fmt;
    } nodeinf;
} _sefNode_t;

/*tp
Format string intermidiate representation

A stateless representation of format
Containing required deplist which Ctx should provides 
and a pointer to an array of nodes
*/
typedef struct {
    const char **deplist;
    _sefNode_t *nodes;
} SEF_FmtIR_t;

/*mod Args
Managing and fetching arguments in formatters
Providing functions to get arguments when writing custom formatters
*/

/*fn 
`SEF_ArgGet` takes `argv` and set `*dst` to the first match of `key`

Returns 1 if a matching was found. Otherwise 0 and leave `*dst` unchanged
*/
int SEF_ArgGet(const SEF_KeyVal_t *argv, const char **dst, const char *key);

/*fn 
`SEF_ArgGetV` takes argv and set each corresponding element in `buf` to the 
first match of the corresponding `key`. I.e buf[i] = Get(argv, key[i])

Returns the number of arguments fetched successfully. 
buf[i] stay unchanged if there wasn't a corresponding arg
*/
int SEF_ArgGetV(const SEF_KeyVal_t *argv, const char **buf, const char **keys);

/*mod Writer
Abstracts write operations
Allows the same interface to write to multiple destinations
*/

/* A available sink types (hidden):
```
enum _sefWriteType {
    _SEF_WTYPE_COUNT = 0,
    _SEF_WTYPE_BUFFER_N,
    _SEF_WTYPE_FILE,
};
```
COUNT: Do nothing, no side effects
BUFFER_N: corresponds to sPrint functions, writes to a buffer of size n, truncates
FILE: corresponds to fPrint and default IPrint functions, writes to standard stream

`SEF_SinkHandler_t` is opaque
*/

/*iufn
`SEF_SinkWrite` writes an entire string `s` to `sink`

Returns the number of bytes written or `strlen(s)` if `sink` is of type COUNT
*/
size_t SEF_SinkWrite(SEF_SinkHandler_t *sink, const char *s);

/*Mod Reg
Manages formatter registry
*/

/*fn 
`SEF_RegistryExists` returns 1 if `specstr` exists in `ctx->registry`
*/
int SEF_RegistryExists(const SEF_Ctx_t *ctx, const char *specstr);
/*fn 
`SEF_RegistryGet` returns the `fn` field in the slot corresponds to `specstr` 
or NULL if it does not exist
*/
SEF_FmtFn_t SEF_RegistryGet(const SEF_Ctx_t *ctx, const char *specstr);

/*mod Context
Manages formatting context 
*/

// blank lol, the type struct is defined in the typedefs section

/*mod Core
Core functionality. That is, formatting
*/

/*fn 
Prints to `stdout` using `ctx` and `ir` resolved with `args`

Returns number of char written
*/
size_t SEF_IPrintf(const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]);
/*fn 
Prints to Nothing using `ctx` and `ir` resolved with `args` (dry-run)

Returns number of char written
*/
size_t SEF_IcPrintf(const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]);
/*fn 
Prints to `stream` using `ctx` and `ir` resolved with `args`

Returns number of char written
*/
size_t SEF_IfPrintf(FILE *stream, const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]);
/*fn 
Prints to `str` buffer of size at least `size` using `ctx` and `ir` resolved with `args`

Returns number of char written
*/
size_t SEF_IsPrintf(char *str, size_t size, const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]);

/*mod IR 
SEF Intermidiate Representation
*/

/* For easy definition of argv */
#define SEF_ARGV(...) ((SEF_KeyVal_t[]){__VA_ARGS__, SEF_KEYVAL_TERM})

/* Begins IR definition with some dependencies */
#define SEF_IR_BEGIN_REQUIRED(...) ((SEF_FmtIR_t){.deplist = (const char*[]){__VA_ARGS__, NULL}, .nodes = (_sefNode_t[]){
/* Begins IR definition without any dependencies */
#define SEF_IR_BEGIN() ((SEF_FmtIR_t){.deplist = (const char*[]){NULL}, .nodes = (_sefNode_t[]){

/* Adds a literal node */
#define SEF_IR_L(str) {.type = _SEFNODE_LTR, .nodeinf.ltr = {str}},
/*
Adds a bfmt node
Begins a format chain. `pos` represents the index in args (in the call of Print),
fmtid represents the index in deplist and argv represents the arguments of the call
*/
#define SEF_IR_BF(pos, fmtid, argv) {.type = _SEFNODE_BFMT, .nodeinf.fmt = {pos, fmtid, argv}},
/*
Adds a pfmt node
Continues a format chain.
fmtid represents the index in deplist and argv represents the arguments of the call

The format chain when the following node is not pfmt
Standalone pfmt is ignored
*/
#define SEF_IR_PF(fmtid, argv) {.type = _SEFNODE_PFMT, .nodeinf.fmt = {0, fmtid, argv}},

/*
Finallize the IR 
*/
#define SEF_IR_END() {.type = _SEFNODE_NULL, {0}}}})

#endif /* SEF_H */
