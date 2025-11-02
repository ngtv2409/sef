#ifndef SEF_WRITER_H
#define SEF_WRITER_H

#include <stddef.h>
typedef struct SEF_SinkHandler SEF_SinkHandler;

// Abstract write function for any sinks
// Use to make formatters
size_t SEF_SinkWrite(SEF_SinkHandler *sink, const char *s);

#endif /* SEF_WRITER_H */
