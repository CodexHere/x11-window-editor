#ifndef LIB_HHUTIL_H
#define LIB_HHUTIL_H

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