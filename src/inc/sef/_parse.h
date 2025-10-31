#pragma once

typedef enum {
    _SEFNODE_LTR = 0, _SEFNODE_FMT
} _sefNodeType_t;

typedef struct {
    int type;
} _sefNodeGeneric_t;

typedef struct {
    int type;
    char *str;
} _sefNodeLTR_t;

typedef struct {
    char *key;
    char *arg;
} _sefArg_t;

typedef struct {
    int type;
    int pos;
    // this id is the order in deplist
    int fmtid;
    _sefArg_t *argv;
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

