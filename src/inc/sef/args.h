#pragma once

#include <stdlib.h> // IWYU pragma: keep

typedef struct {
    const char *key;
    const char *val;
} SEF_KeyVal_t;
#define SEF_KeyVal_TERM ((SEF_KeyVal_t){NULL, NULL})

int SEF_ArgGet(const SEF_KeyVal_t argv[], const char **dst, const char *key);
int SEF_ArgGetV(const SEF_KeyVal_t argv[], const char *buf[], const char *keys[]);
