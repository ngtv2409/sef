#pragma once

#include <stddef.h>
#include <stdio.h>
enum _sefWriteType {
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
