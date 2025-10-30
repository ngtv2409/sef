#pragma once

#include <stddef.h>
typedef struct SEF_SinkHandler SEF_SinkHandler;

// Abstract write function for any sinks
// Use to make formatters
size_t SEF_SinkWrite(SEF_SinkHandler *sink, const char *s);
