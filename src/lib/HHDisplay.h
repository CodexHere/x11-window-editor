#ifndef LIB_HHDISPLAY_H
#define LIB_HHDISPLAY_H

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