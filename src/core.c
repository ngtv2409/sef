/*
 > Module Core
 @ SEF's main functionalities
 | Implements format functions, sinks etc. To provides printf-like interface

 TODO: Since VLA is eliminated, recursion is no longer mandatory for fmtchain 
 Use iterative approach by using 2 alternating buffers (so the stack will not explode)
*/

#include "sef/_writer.h"
#include "sef/sef.h"
#include "sef/limits.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t SEF_SinkWrite(SEF_SinkHandler_t *sink, const char *s);
SEF_FmtFn_t SEF_RegistryGet(const SEF_Ctx_t *ctx, const char *specstr);

static size_t _sefFmtChainRecursive(char *wbuffer, const void *arg,
                                    const _sefNode_t *nodes, SEF_FmtFn_t *depfn,
                                    size_t i, size_t j, int depth) {
    if (depth >= SEF_LIM_MAXPIPE) return SEF_Err; 
    char buf[SEF_LIM_MAXINTSTR];
    _sefSinkHandler_t tmpsink = {
        _SEF_WTYPE_BUFFER_N, {.buf = {buf, 0, SEF_LIM_MAXINTSTR}}
    };
    depfn[nodes[i].nodeinf.fmt.fmtid]
        ((SEF_SinkHandler_t*)&tmpsink, arg, nodes[i].nodeinf.fmt.argv);

    if (i + 1 >= j) {
        // the end
        size_t len = strlen(buf);
        if (wbuffer) {
            strncpy(wbuffer, buf, len + 1);
            return len;
        } else {
            return strlen(buf);
        }
    }
    return _sefFmtChainRecursive(wbuffer, (void*)buf, nodes, depfn, i + 1, j, depth + 1);
}

size_t _sefFmtIR(const SEF_Ctx_t *ctx, SEF_SinkHandler_t *sink,
                 const SEF_FmtIR_t *ir, const void *args[]) {
    size_t n = 0;
    const _sefNode_t *nodes = ir->nodes;
    const char **deplist = ir->deplist;

    size_t depcnt;
    for (depcnt = 0; deplist[depcnt]; ++depcnt);
    if (depcnt >= SEF_LIM_MAXDEP) return SEF_Err;
    SEF_FmtFn_t depfn[SEF_LIM_MAXDEP];
    size_t i;
    for (i = 0; i < depcnt; ++i) {
        SEF_FmtFn_t f = SEF_RegistryGet(ctx, deplist[i]);
        if (NULL == f) {
            return SEF_Err;
        }
        depfn[i] = f;
    }
    depfn[i] = NULL;


    // execution
    // only alloc non-literal otherwise just ref
    for (size_t i = 0; nodes[i].type != _SEFNODE_NULL; ++i) {
        switch (nodes[i].type) {
            case _SEFNODE_LTR:
                n += SEF_SinkWrite(sink, nodes[i].nodeinf.ltr.str);
                break;
            case _SEFNODE_BFMT: {
                (void)0;
                const void *arg = args[nodes[i].nodeinf.fmt.pos];
                size_t pfmtend;
                for (pfmtend = i + 1;
                            nodes[pfmtend].type != _SEFNODE_NULL &&
                            nodes[pfmtend].type == _SEFNODE_PFMT;
                    ++pfmtend);
                char buf[SEF_LIM_MAXINTSTR];
                if (_sefFmtChainRecursive(buf, arg, nodes, depfn,
                                      i, pfmtend, 0) == SEF_Err) {
                    return SEF_Err;
                }
                n += SEF_SinkWrite(sink, buf);
                i = pfmtend - 1;
                break;
            }
            default:
            // IR is internal so leave invalid IR undefined
                (void)0;
        }
    }
    return n;
}

size_t SEF_IPrintf(const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]) {
    _sefSinkHandler_t sink = {_SEF_WTYPE_FILE, {.file = {stdout}}};
    return _sefFmtIR(ctx, (SEF_SinkHandler_t*)&sink, ir, args);
}

size_t SEF_IcPrintf(const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]) {
    _sefSinkHandler_t sink = {_SEF_WTYPE_COUNT, {0}};
    return _sefFmtIR(ctx, (SEF_SinkHandler_t*)&sink, ir, args);
}

size_t SEF_IfPrintf(FILE *stream, const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]) {
    _sefSinkHandler_t sink = {_SEF_WTYPE_FILE, {.file = {stream}}};
    return _sefFmtIR(ctx, (SEF_SinkHandler_t*)&sink, ir, args);
}
size_t SEF_IsPrintf(char *str, size_t size, const SEF_Ctx_t *ctx, const SEF_FmtIR_t *ir, const void *args[]) {
    _sefSinkHandler_t sink = {_SEF_WTYPE_BUFFER_N, {.buf = {str, 0, size}}};
    return _sefFmtIR(ctx, (SEF_SinkHandler_t*)&sink, ir, args);
}
