#include "sef/_parse.h"
#include "sef/_writer.h"
#include "sef/_pubtypes.h"

#include "sef/error.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t SEF_SinkWrite(SEF_SinkHandler *sink, const char *s);
SEF_FmtFn_t SEF_RegistryGet(SEF_Ctx_t *ctx, const char *specstr);

static size_t _sefFmtChainRecursive(char *wbuffer, void *arg,
                                    _sefNodeFMT_t *nodes[], SEF_FmtFn_t depfn[],
                                    size_t i, size_t j) {
    
    _sefSinkHandler cntsink = {_SEF_WTYPE_COUNT, {}};
    size_t len = depfn[((_sefNodeFMT_t*)(nodes[i]))->fmtid]
        ((SEF_SinkHandler*)&cntsink, arg, (SEF_KeyVal_t*)((_sefNodeFMT_t*)nodes[i])->argv);
    char buf[len + 1];
    _sefSinkHandler tmpsink = {
        _SEF_WTYPE_BUFFER_N, {.buf = {buf, 0, len + 1}}
    };
    depfn[((_sefNodeFMT_t*)(nodes[i]))->fmtid]
        ((SEF_SinkHandler*)&tmpsink, arg, (SEF_KeyVal_t*)((_sefNodeFMT_t*)nodes[i])->argv);

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

size_t _sefFmtIR(SEF_Ctx_t *ctx, SEF_SinkHandler *sink,
                SEF_FmtIR_t *_ir, void *args[]) {
    size_t n = 0;
    _sefFmtIR_t *ir = (_sefFmtIR_t*)_ir;
    _sefNodeGeneric_t **nodes = ir->nodes;
    char **deplist = ir->deplist;

    size_t depcnt;
    for (depcnt = 0; deplist[depcnt]; ++depcnt);
    SEF_FmtFn_t depfn[depcnt];
    for (size_t i = 0; i < depcnt; ++i) {
        SEF_FmtFn_t f = SEF_RegistryGet(ctx, deplist[i]);
        if (NULL == f) {
            SEF_Errno = SEF_ErrRegFnNotFound;
            return 0;
        }
        depfn[i] = f;
    }

    // execution
    // only alloc non-literal otherwise just ref
    for (size_t i = 0; nodes[i]; ++i) {
        switch (nodes[i]->type) {
            case _SEFNODE_LTR:
                n += SEF_SinkWrite(sink ,((_sefNodeLTR_t*)nodes[i])->str);
                break;
            case _SEFNODE_BFMT: {
                (void)0;
                void *arg = args[((_sefNodeFMT_t*)(nodes[i]))->pos];
                size_t pfmtend;
                for (pfmtend = i + 1; nodes[pfmtend] &&
                            nodes[pfmtend]->type == _SEFNODE_PFMT; ++pfmtend);
                size_t len = _sefFmtChainRecursive(NULL, arg, (_sefNodeFMT_t**)nodes, depfn,
                                                   i, pfmtend);
                char buf[len + 1];
                _sefFmtChainRecursive(buf, arg, (_sefNodeFMT_t**)nodes, depfn,
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
    _sefSinkHandler sink = {_SEF_WTYPE_FILE, {.file = {stdout}}};
    return _sefFmtIR(ctx, (SEF_SinkHandler*)&sink, ir, args);
}
