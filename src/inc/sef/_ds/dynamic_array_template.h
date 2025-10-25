// template header
#pragma once

#include <stdlib.h> // IWYU pragma: keep

// minimal dynamic stack template
// only need Push since it's mostly for internal buffers

#define __SEFDS_IMPORT_DStack_IMPL(ds, T)\
int ds##_Alloc(ds *cont, size_t init_cap) {\
    T *buf = malloc(init_cap * sizeof(T));\
    if (!buf) return 1;\
    cont->buf = buf;\
    cont->size = 0;\
    cont->cap = init_cap;\
    return 0;\
}\
void ds##_Free(ds *cont) {\
    free(cont->buf);\
}\
int ds##_Push(ds *cont, T val) {\
    if (cont->size >= cont->cap) {\
        T *temp = realloc(cont->buf, cont->cap * 2 * sizeof(T));\
        if (!temp) return 1;\
        cont->buf = temp;\
        cont->cap *= 2;\
    }\
    (cont->buf)[cont->size] = val;\
    ++(cont->size);\
    return 0;\
}

#define __SEFDS_IMPORT_DStack_DEF(ds, T)\
typedef struct {\
    T *buf;\
    size_t size, cap;\
} ds;\
int ds##_Alloc(ds *cont, size_t init_cap);\
void ds##_Free(ds *cont);\
int ds##_Push(ds *cont, T val);
