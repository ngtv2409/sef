/*
 @ Internal usage of Public types
 ! Must be included before sef.h if both are needed
*/

#ifndef _SEF_PUBTYPES_H
#define _SEF_PUBTYPES_H

#include <stddef.h>

//@opaque
typedef struct SEF_SinkHandler SEF_SinkHandler;
//@opaque
typedef struct SEF_FmtIR_t SEF_FmtIR_t;

typedef struct {
    const char *key, *val;
} SEF_KeyVal_t;
#define SEF_KEYVAL_TERM ((SEF_KeyVal_t){NULL, NULL})
typedef size_t (*SEF_FmtFn_t)(SEF_SinkHandler *, const void *, const SEF_KeyVal_t *);
typedef struct {
    const char *key;
    SEF_FmtFn_t fn;
} SEF_RegistrySlot_t;
#define SEF_REGSLOT_TERM ((SEF_RegistrySlot_t){NULL, NULL})
typedef struct {
    SEF_RegistrySlot_t *registry;
} SEF_Ctx_t;

#endif /* _SEF_PUBTYPES_H */
