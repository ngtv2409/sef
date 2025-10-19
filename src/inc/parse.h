#pragma once

/* Use to reference literal content without copying (immutable) */
typedef struct {
    const int len;
    const char *str;
} _sefImStr;

/* IR node type 
 * - LTL: string literal
 * - PFMT: piped format (the output will be send to the next node)
 * Note: PFMT expected FMT as the next node
 * - FMT: format
 */
typedef enum {
    LTR, PFMT, FMT
} _sefNodeType;

typedef struct {
    _sefNodeType type;
} _sefNodeGeneric;

typedef struct {
    _sefNodeType type;
    _sefImStr str;
} _sefNodeLTR;

typedef struct {
    _sefImStr key;
    _sefImStr arg;
} _sefArg;

/* Both FMT and PFMT take the same structure differ by type */
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
    char **deplist;
    _sefNodeGeneric *ir;
} _sefFmtIR;

_sefFmtIR _sefFmtParse(char *fmtstr);

