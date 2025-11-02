/*
 > Module Args
 @ Manages KeyVal arrays to fetch arguments
*/

#include "sef/_pubtypes.h"

#include <stddef.h>
#include <string.h>


int SEF_ArgGet(const SEF_KeyVal_t argv[], const char **dst, const char *key) {
    while (argv->key) {
        if (strcmp(argv->key, key) == 0) {
            *dst = argv->val;
            return 1;
        }
        ++argv;
    }
    return 0;
}

int SEF_ArgGetV(const SEF_KeyVal_t argv[], const char *buf[], const char *keys[]) {
    int n = 0;
    for (size_t i = 0; keys[i]; ++i) {
        n += SEF_ArgGet(argv, &buf[i], keys[i]);
    }
    return n;
}
