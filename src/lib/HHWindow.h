#ifndef _LIB_HHWINDOW_H_
#define _LIB_HHWINDOW_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xatom.h>

// Window Hints :

typedef struct
{
    // Setting and Reading the WM_NORMAL_HINTS Property:
    //      https://tronche.com/gui/x/xlib/ICC/client-to-window-manager/wm-normal-hints.html
    void (*set_fixed_size)(Window window_id, char *width, char *height);
    void (*undo_fixed_size)(Window window_id);

    // XChangeProperty Function:
    //      https://tronche.com/gui/x/xlib/window-information/XChangeProperty.html
    // All Props:
    //      https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863921328

    void (*set_property)(Window window_id, char *property_name, char *atom_names[], int num_atoms);

    // _NET_WM_STATE: https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863892896
    void (*send_event)(Window window_id, char *event_name, char *atom_names[], int num_atoms, Bool add);
} NSHHWindow;

extern NSHHWindow HHWindow;

#endif