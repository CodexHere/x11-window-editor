#ifndef _LIB_HHUTIL_H_
#define _LIB_HHUTIL_H_

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char *(*trim)(const char *str);
    char **(*delimit)(char *input, char *delimiter, int *num_items);
} NSHHUtil;

extern NSHHUtil HHUtil;

#endif