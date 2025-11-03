/*
 > Simple Extensible Formatter 
 @ Extensible format library
 | A format library which aims to be extensible via user-defined functions
 | while prioritize efficiency and simplicity.
TODO: implement BYOM pattern
*/

#ifndef SEF_H
#define SEF_H

// for usage of NULL in macros
#include <stdlib.h> // IWYU pragma: keep
// for size_t
#include <stddef.h>

/* Typedefs */
/*
   defines only if _pubtypes.h is not included to avoid redefinition
   this is for debug when inclusion of both api internal headers are necessary
*/
#ifndef _SEF_PUBTYPES_H
//@opaque
typedef struct SEF_Ctx_t SEF_Ctx_t;
//@opaque
typedef struct SEF_SinkHandler SEF_SinkHandler;
//@opaque
typedef struct SEF_FmtIR_t SEF_FmtIR_t;
//@opaque
typedef struct SEF_KeyVal_t SEF_KeyVal_t;

typedef size_t (*SEF_FmtFn_t)(SEF_SinkHandler *, const void *, const SEF_KeyVal_t *);

typedef struct {
    const char *key;
    SEF_FmtFn_t fn;
} SEF_RegistrySlot_t;
#define SEF_REGSLOT_TERM ((SEF_RegistrySlot_t){NULL, NULL})
#endif

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
size_t SEF_SinkWrite(SEF_SinkHandler *sink, const char *s);

/* 
 > Module Reg
 @ Manages formatter registry
*/
int SEF_RegistryExists(SEF_Ctx_t *ctx, const char *specstr);
SEF_FmtFn_t SEF_RegistryGet(SEF_Ctx_t *ctx, const char *specstr);

/*
 > Module Context
 @ Manages formatting context 
*/
void SEF_CtxInit(SEF_Ctx_t *_ctx, SEF_RegistrySlot_t *reg);

/*
 > Module Core
 @ SEF's main functionalities
 | Implements format functions, sinks etc. To provide printf-like interface
*/
size_t SEF_IPrintf(SEF_Ctx_t *ctx, SEF_FmtIR_t *ir, void *args[]);

#endif /* SEF_H */
