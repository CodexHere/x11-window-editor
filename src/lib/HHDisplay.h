#ifndef _LIB_HHDISPLAY_H_
#define _LIB_HHDISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>

typedef struct
{
    Display *(*attach)();
    void (*detach)(Display *display);
} NSHHDisplay;

extern NSHHDisplay HHDisplay;

#endif