/*
 > Simple Extensible Formatter 
 @ Extensible format library
 | A format library which aims to be extensible via user-defined functions
 | while prioritize efficiency and simplicity.
*/

#ifndef SEF_H
#define SEF_H

// for usage of NULL in macros
#include <stdio.h>
#include <stdlib.h> // IWYU pragma: keep
// for size_t
#include <stddef.h>

/* @ For size_t returned functions, max is reserved for errors */
#define SEF_Err ((size_t)~0)

/* Typedefs */
/*
   defines only if _pubtypes.h is not included to avoid redefinition
   this is for debug when inclusion of both api internal headers are necessary
*/
//@opaque
typedef struct SEF_SinkHandler_t SEF_SinkHandler_t;

typedef struct {
    const char *key, *val;
} SEF_KeyVal_t;
#define SEF_KEYVAL_TERM ((SEF_KeyVal_t){NULL, NULL})

typedef size_t (*SEF_FmtFn_t)(SEF_SinkHandler_t *, const void *, const SEF_KeyVal_t *);

typedef struct {
    const char *key;
    SEF_FmtFn_t fn;
} SEF_RegistrySlot_t;
#define SEF_REGSLOT_TERM ((SEF_RegistrySlot_t){NULL, NULL})

typedef struct {
    const SEF_RegistrySlot_t *registry;
} SEF_Ctx_t;

/* @ IR */
typedef enum {
    _SEFNODE_NULL = 0, _SEFNODE_LTR , _SEFNODE_BFMT, _SEFNODE_PFMT
} _sefNodeType_t;

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

/*
 * Format string intermidiate representation
 * Parser output
 */
typedef struct {
    const char **deplist;
    _sefNode_t *nodes;
} SEF_FmtIR_t;

/*
 > Module Args
 @ Manages KeyVal arrays to fetch arguments
*/
int SEF_ArgGet(const SEF_KeyVal_t *argv, const char **dst, const char *key);
int SEF_ArgGetV(const SEF_KeyVal_t *argv, const char **buf, const char **keys);

/*
 > Module Writer
 @ Abstracts write operations
 | Allows the same interface to write to multiple destinations
*/
size_t SEF_SinkWrite(SEF_SinkHandler_t *sink, const char *s);

/* 
 > Module Reg
 @ Manages formatter registry
*/
int SEF_RegistryExists(const SEF_Ctx_t *ctx, const char *specstr);
SEF_FmtFn_t SEF_RegistryGet(const SEF_Ctx_t *ctx, const char *specstr);

/*
 > Module Context
 @ Manages formatting context 
*/

// blank lol, the type struct is defined in the typedefs section

/*
 > Module Core
 @ SEF's main functionalities
 | Implements format functions, sinks etc. To provide printf-like interface
*/

/* IPrint (Print using IR as format) */

size_t SEF_IPrintf(const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]);

/* @ Prints dry run (count size) */
size_t SEF_IcPrintf(const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]);
/* @ Prints to a stream */
size_t SEF_IfPrintf(FILE *stream, const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]);
/* @ Prints to buffer (analogous to snprintf rather than sprintf (unsafe)) */
size_t SEF_IsPrintf(char *str, size_t size, const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]);

/*
 > Module IR 
 @ SEF IR backend

 ! SEF exposes IR rather than make it opaque
 ! This allows IR creation via macros
 ! So think twice before touching what you ain't supposed to
 ! A string frontend will not be available anytime soon
*/

#define SEF_ARGV(...) ((SEF_KeyVal_t[]){__VA_ARGS__, SEF_KEYVAL_TERM})

#define SEF_IR_BEGIN(...) ((SEF_FmtIR_t){.deplist = (const char*[]){__VA_ARGS__, NULL}, .nodes = (_sefNode_t[]){
#define SEF_IR_L(str) {.type = _SEFNODE_LTR, .nodeinf.ltr = {str}},
#define SEF_IR_BF(pos, fmtid, argv) {.type = _SEFNODE_BFMT, .nodeinf.fmt = {pos, fmtid, argv}},
#define SEF_IR_PF(fmtid, argv) {.type = _SEFNODE_PFMT, .nodeinf.fmt = {0, fmtid, argv}},
#define SEF_IR_END() {.type = _SEFNODE_NULL, {0}}}})

#endif /* SEF_H */
