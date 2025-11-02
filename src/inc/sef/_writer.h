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
} _sefSinkBuf;
typedef struct {
    FILE *f;
} _sefSinkFile;

typedef struct {
    int wtype;
    union {
        _sefSinkBuf buf;
        _sefSinkFile file;
    } sink;
} _sefSinkHandler;

#endif /* _SEF_WRITER_H */
