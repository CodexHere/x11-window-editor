#ifndef LIB_HHWINDOW_H
#define LIB_HHWINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xatom.h>

typedef enum
{
    HH_EVENT_MODE_REMOVE = 0,
    HH_EVENT_MODE_ADD = 1,
    HH_EVENT_MODE_TOGGLE = 2,
    HH_EVENT_MODE_MINIMIZE = IconicState
} HH_EVENT_MODE;

typedef struct
{
    void (*minimize)(Window window_id);
    void (*maximize)(Window window_id);

    void (*toggleFixedSize)(Window window_id, Bool isEnabled, int width, int height);
    void (*toggleSticky)(Window window_id, Bool isEnabled);
    void (*toggleShade)(Window window_id, Bool isEnabled);
    void (*toggleTaskbar)(Window window_id, Bool isEnabled);
    void (*togglePager)(Window window_id, Bool isEnabled);
    void (*toggleFullscreen)(Window window_id, Bool isEnabled);
    void (*toggleAbove)(Window window_id, Bool isEnabled);
    void (*toggleBelow)(Window window_id, Bool isEnabled);

    void (*restore)(Window window_id);

    // XChangeProperty Function:
    //      https://tronche.com/gui/x/xlib/window-information/XChangeProperty.html
    // All Props:
    //      https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863921328

    void (*set_property)(Window window_id, char *property_name, char *atom_names[], int num_atoms);

    // _NET_WM_STATE: https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863892896
    void (*send_event)(Window window_id, char *event_name, char *atom_names[], int num_atoms, HH_EVENT_MODE mode);
} NSHHWindow;

extern NSHHWindow HHWindow;

#endif