/*
 > Module Writer
 @ Abstracts write operations
 | Allows the same interface to write to multiple destinations
*/

#include "sef/_writer.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

typedef struct SEF_SinkHandler_t SEF_SinkHandler_t;

size_t SEF_SinkWrite(SEF_SinkHandler_t *_sink, const char *s) {
    _sefSinkHandler_t *sink = (_sefSinkHandler_t*)_sink;
    size_t n = 0;
    if (sink->wtype == _SEF_WTYPE_COUNT) {
        return strlen(s);
    } else if (sink->wtype == _SEF_WTYPE_BUFFER_N) {
        _sefSinkBuf_t inf = sink->sink.buf;
        while (inf.cur < inf.size - 1) {
            if (!s[n]) break;
            if (inf.buf) {
                inf.buf[inf.cur] = s[n];
            }
            ++n;
            ++inf.cur;
        };
        if (inf.buf) {
            inf.buf[inf.cur] = '\0';
        }
        return n;
    } else if (sink->wtype == _SEF_WTYPE_FILE) {
        return fwrite(s, sizeof(char), strlen(s), sink->sink.file.f);
    } else {
        // sentinel value (max of size_t)
        return SIZE_T_MAX;
    }
}
