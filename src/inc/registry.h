#pragma once

/* Format function arguments
 * Key-Value string pair */
typedef struct {
     char *key, *value;
} SEF_FmtFnArg_t;

typedef char *(*SEF_FmtFn_t)(const char *, SEF_FmtFnArg_t *);

int SEF_RegistryAdd(const char *specstr, SEF_FmtFn_t fn);
int SEF_RegistryExists(const char *specstr);
SEF_FmtFn_t SEF_RegistryGet(const char *specstr);
