#pragma once
#include "sef/_warntype.h"

/* IR node type 
 * - LTL: string literal
 * - PFMT: piped format (the output will be send to the next node)
 * Note: PFMT expected FMT as the next node
 * - FMT: format
 */
typedef enum {
    _SEFNODE_LTR = 0, _SEFNODE_PFMT, _SEFNODE_FMT
} _sefNodeType_t;

typedef struct {
    int type;
} _sefNodeGeneric_t;

typedef struct {
    int type;
    _sefStrAlloc_t *str;
} _sefNodeLTR_t;

typedef struct {
    _sefStrAlloc_t key;
    _sefStrAlloc_t *arg;
} _sefArg_t;

/* Both FMT and PFMT take the same structure differ by type
 * pos is only required in the first PFMT, FMT signates the end */
typedef struct {
    int type;
    int pos;
    // this id is the order in deplist
    int fmtid;
    _sefArg_t argv[];
} _sefNodeFMT_t;

/*
 * Format string intermidiate representation
 * Parser output
 */
typedef struct {
    char **deplist;
    // strs in IR are Offset to buf
    _sefNodeGeneric_t *ir;
} _sefFmtIR_t;

_sefFmtIR_t _sefFmtParse(const char *fmtstr);

