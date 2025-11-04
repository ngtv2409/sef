/*
 > Internal writer 
 @ abstracts write destinations
*/

#ifndef _SEF_WRITER_H
#define _SEF_WRITER_H

#include <stddef.h>
#include <stdio.h>

enum _sefWriteType {
    _SEF_WTYPE_COUNT = 0,
    _SEF_WTYPE_BUFFER_N,
    _SEF_WTYPE_FILE,
};
typedef struct {
    char *buf;
    size_t cur;
    size_t size;
} _sefSinkBuf_t;
typedef struct {
    FILE *f;
} _sefSinkFile_t;

typedef struct {
    int wtype;
    union {
        _sefSinkBuf_t buf;
        _sefSinkFile_t file;
    } sink;
} _sefSinkHandler_t;

#endif /* _SEF_WRITER_H */
