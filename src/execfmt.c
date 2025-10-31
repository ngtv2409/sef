#include "sef/execfmt.h"
#include "sef/_parse.h"
#include "sef/context.h"
#include "sef/error.h"
#include "sef/registry.h"
#include "sef/writer.h"
#include "sef/_writer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
            case _SEFNODE_FMT:
                (void)0;
                void *arg = args[((_sefNodeFMT_t*)(nodes[i]))->pos];
                n += depfn[((_sefNodeFMT_t*)(nodes[i]))->fmtid](sink, arg, 
                      (SEF_FmtFnArg_t*)((_sefNodeFMT_t*)nodes[i])->argv);
                break;
        }
    }
    return n;
}

size_t SEF_IPrintf(SEF_Ctx_t *ctx, SEF_FmtIR_t *ir, void *args[]) {
    _sefSinkHandler sink = {_SEF_WTYPE_FILE, {.file = {stdout}}};
    return _sefFmtIR(ctx, (SEF_SinkHandler*)&sink, ir, args);
}
