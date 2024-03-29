#ifndef LIB_HHHELP_H
#define LIB_HHHELP_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#define COLOR_RED "\033[91m"
#define COLOR_GREEN "\033[32m"
#define COLOR_GREY "\033[90m"
#define COLOR_RESET "\033[0m"

typedef enum
{
    ACTION_NONE,

    ACTION_RAW_PROP,
    ACTION_RAW_EVENT,

    ACTION_MAXIMIZE,
    ACTION_MINIMIZE,
    ACTION_RESTORE,
    ACTION_RAISE,
    ACTION_MOVE,
    ACTION_SIZE,

    ACTION_SET_TITLE,
    ACTION_SET_ROLE,
    ACTION_SET_WINDOW_TYPE,
    ACTION_SET_CLASS,
    ACTION_SET_CLASSNAME,

    ACTION_TOGGLE_FIXED_SIZE,
    ACTION_TOGGLE_ABOVE,
    ACTION_TOGGLE_BELOW,
    ACTION_TOGGLE_SHADE,
    ACTION_TOGGLE_STICKY,
    ACTION_TOGGLE_TASKBAR,
    ACTION_TOGGLE_PAGER,
    ACTION_TOGGLE_FULLSCREEN
} Action;

void help(int exit_code, char *error_message);

extern char *script_name;
extern struct option long_options[];

#endif