#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#define logger(line) fprintf(stderr, "%s:%d [%s]: %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, line)
#define loggerf(format, ...) fprintf(stderr, "%s:%d [%s]: " format "\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__)

#endif

