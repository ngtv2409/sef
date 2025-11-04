/*
 > Module Core
 @ SEF's main functionalities
 | Implements format functions, sinks etc. To provides printf-like interface
*/

#include "sef/_writer.h"
#include "sef/sef.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t SEF_SinkWrite(SEF_SinkHandler_t *sink, const char *s);
SEF_FmtFn_t SEF_RegistryGet(SEF_Ctx_t *ctx, const char *specstr);

static size_t _sefFmtChainRecursive(char *wbuffer, void *arg,
                                    _sefNode_t *nodes, SEF_FmtFn_t *depfn,
                                    size_t i, size_t j) {
    
    _sefSinkHandler_t cntsink = {_SEF_WTYPE_COUNT, {0}};
    size_t len = depfn[nodes[i].nodeinf.fmt.fmtid]
        ((SEF_SinkHandler_t*)&cntsink, arg, nodes[i].nodeinf.fmt.argv);
    char buf[len + 1];
    _sefSinkHandler_t tmpsink = {
        _SEF_WTYPE_BUFFER_N, {.buf = {buf, 0, len + 1}}
    };
    depfn[nodes[i].nodeinf.fmt.fmtid]
        ((SEF_SinkHandler_t*)&tmpsink, arg, nodes[i].nodeinf.fmt.argv);

    if (i + 1 >= j) {
        // the end
        if (wbuffer) {
            strncpy(wbuffer, buf, len + 1);
            return len;
        } else {
            return strlen(buf);
        }
    }
    return _sefFmtChainRecursive(wbuffer, (void*)buf, nodes, depfn, i + 1, j);
}

size_t _sefFmtIR(SEF_Ctx_t *ctx, SEF_SinkHandler_t *sink,
                SEF_FmtIR_t *ir, void *args[]) {
    size_t n = 0;
    _sefNode_t *nodes = ir->nodes;
    const char **deplist = ir->deplist;

    size_t depcnt;
    for (depcnt = 0; deplist[depcnt]; ++depcnt);
    SEF_FmtFn_t depfn[depcnt];
    for (size_t i = 0; i < depcnt; ++i) {
        SEF_FmtFn_t f = SEF_RegistryGet(ctx, deplist[i]);
        if (NULL == f) {
            return 0;
        }
        depfn[i] = f;
    }

    // execution
    // only alloc non-literal otherwise just ref
    for (size_t i = 0; nodes[i].type != _SEFNODE_NULL; ++i) {
        switch (nodes[i].type) {
            case _SEFNODE_LTR:
                n += SEF_SinkWrite(sink, nodes[i].nodeinf.ltr.str);
                break;
            case _SEFNODE_BFMT: {
                (void)0;
                void *arg = args[nodes[i].nodeinf.fmt.pos];
                size_t pfmtend;
                for (pfmtend = i + 1;
                            nodes[pfmtend].type != _SEFNODE_NULL &&
                            nodes[pfmtend].type == _SEFNODE_PFMT;
                    ++pfmtend);
                size_t len = _sefFmtChainRecursive(NULL, arg, nodes, depfn,
                                                   i, pfmtend);
                char buf[len + 1];
                _sefFmtChainRecursive(buf, arg, nodes, depfn,
                                      i, pfmtend);
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

size_t SEF_IPrintf(SEF_Ctx_t *ctx, SEF_FmtIR_t *ir, void *args[]) {
    _sefSinkHandler_t sink = {_SEF_WTYPE_FILE, {.file = {stdout}}};
    return _sefFmtIR(ctx, (SEF_SinkHandler_t*)&sink, ir, args);
}

size_t SEF_IcPrintf(SEF_Ctx_t *ctx, SEF_FmtIR_t *ir, void *args[]) {
    _sefSinkHandler_t sink = {_SEF_WTYPE_COUNT, {0}};
    return _sefFmtIR(ctx, (SEF_SinkHandler_t*)&sink, ir, args);
}

size_t SEF_IfPrintf(FILE *stream, SEF_Ctx_t *ctx, SEF_FmtIR_t *ir, void *args[]) {
    _sefSinkHandler_t sink = {_SEF_WTYPE_FILE, {.file = {stream}}};
    return _sefFmtIR(ctx, (SEF_SinkHandler_t*)&sink, ir, args);
}
size_t SEF_IsPrintf(char *str, size_t size, SEF_Ctx_t *ctx, SEF_FmtIR_t *ir, void *args[]) {
    _sefSinkHandler_t sink = {_SEF_WTYPE_BUFFER_N, {.buf = {str, 0, size}}};
    return _sefFmtIR(ctx, (SEF_SinkHandler_t*)&sink, ir, args);
}
