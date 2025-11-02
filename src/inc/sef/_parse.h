#ifndef _SEF_PARSE_H
#define _SEF_PARSE_H

#include "sef/args.h"

typedef enum {
    _SEFNODE_LTR = 0, _SEFNODE_BFMT, _SEFNODE_PFMT
} _sefNodeType_t;

typedef struct {
    int type;
} _sefNodeGeneric_t;

typedef struct {
    int type;
    char *str;
} _sefNodeLTR_t;

typedef struct {
    int type;
    int pos;
    // this id is the order in deplist
    int fmtid;
    SEF_KeyVal_t *argv;
} _sefNodeFMT_t;

/*
 * Format string intermidiate representation
 * Parser output
 */
typedef struct {
    char **deplist;
    _sefNodeGeneric_t **nodes;
} _sefFmtIR_t;

_sefFmtIR_t _sefFmtParse(const char *fmtstr);


#endif /* _SEF_PARSE_H */
