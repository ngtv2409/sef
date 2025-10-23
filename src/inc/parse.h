#pragma once
#include "warntype.h"

/* IR node type 
 * - LTL: string literal
 * - PFMT: piped format (the output will be send to the next node)
 * Note: PFMT expected FMT as the next node
 * - FMT: format
 */
typedef enum {
    _SEFNODE_LTR, _SEFNODE_PFMT, _SEFNODE_FMT
} _sefNodeType;

typedef struct {
    _sefNodeType type;
} _sefNodeGeneric;

typedef struct {
    _sefNodeType type;
    _sefStrOffset *str;
} _sefNodeLTR;

typedef struct {
    _sefStrOffset key;
    _sefStrOffset *arg;
} _sefArg;

/* Both FMT and PFMT take the same structure differ by type
 * pos is only required in the first PFMT, FMT signates the end */
typedef struct {
    _sefNodeType type;
    int pos;
    // this id is the order in deplist
    int fmtid;
    _sefArg argv[];
} _sefNodeFMT;

/*
 * Format string intermidiate representation
 * Parser output
 */
typedef struct {
    // Pointer used to free every strings in _sefFmtIR
    // in destructor, generated from lexor output
    _sefStrAlloc buf;

    char **deplist;
    // strs in IR are Offset to buf
    _sefNodeGeneric *ir;
} _sefFmtIR;

_sefFmtIR _sefFmtParse(const char *fmtstr);

