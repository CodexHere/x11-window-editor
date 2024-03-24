#ifndef _LIB_HHWINDOW_H_
#define _LIB_HHWINDOW_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xatom.h>

#define HH_EVENT_MODE_REMOVE 0
#define HH_EVENT_MODE_ADD 1
#define HH_EVENT_MODE_TOGGLE 2
#define HH_EVENT_MODE_MINIMIZE IconicState

typedef struct
{
    void (*minimize)(Window window_id);
    void (*maximize)(Window window_id);

    void (*toggleFixedSize)(Window window_id, Bool enable, char *width, char *height);
    void (*toggleSticky)(Window window_id, long mode);
    void (*toggleShade)(Window window_id, long mode);
    void (*toggleTaskbar)(Window window_id, long mode);
    void (*togglePager)(Window window_id, long mode);
    void (*toggleFullscreen)(Window window_id, long mode);
    void (*toggleAbove)(Window window_id, long mode);
    void (*toggleBelow)(Window window_id, long mode);

    void (*restore)(Window window_id);

    // XChangeProperty Function:
    //      https://tronche.com/gui/x/xlib/window-information/XChangeProperty.html
    // All Props:
    //      https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863921328

    void (*set_property)(Window window_id, char *property_name, char *atom_names[], int num_atoms);

    // _NET_WM_STATE: https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863892896
    void (*send_event)(Window window_id, char *event_name, char *atom_names[], int num_atoms, long mode);
} NSHHWindow;

extern NSHHWindow HHWindow;

#endif