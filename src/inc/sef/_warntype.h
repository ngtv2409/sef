#pragma once

/*
 * This header contains type aliases and macros 
 * which improve the clarity about what the type is, how it should be use
 * whether it is allocated or freeable or just a pointer
 * to an offset of another str */

// str is allocated by malloc, could be free
typedef char* _sefStrAlloc_t;
// str is allocated on the heap but is not the root pointer, cannot be freed
typedef char* _sefStrOffset_t;
