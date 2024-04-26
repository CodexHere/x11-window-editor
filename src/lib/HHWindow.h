#ifndef LIB_HHWINDOW_H
#define LIB_HHWINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

typedef enum
{
    HH_EVENT_MODE_REMOVE = 0,
    HH_EVENT_MODE_ADD = 1,
    HH_EVENT_MODE_TOGGLE = 2,
    HH_EVENT_MODE_MINIMIZE = IconicState
} HH_EVENT_MODE;

typedef enum
{
    HH_PROPERTY_LEN_MODE_ATOMIZED = 0,
    HH_PROPERTY_LEN_MODE_CARDINAL = 1,
    HH_PROPERTY_LEN_MODE_RAW = 2
} HH_PROPERTY_LEN_MODE;

typedef struct
{
    void (*minimize)(Window window_id);
    void (*maximize)(Window window_id);
    void (*restore)(Window window_id);
    void (*raise)(Window window_id);
    void (*move)(Window window_id, int x, int y);
    void (*size)(Window window_id, int width, int height);

    XSizeHints *(*get_normal_hints)(Display *display, Window window_id);
    XWindowAttributes *(*get_attributes)(Display *display, Window window_id);
    XClassHint *(*get_class_hints)(Display *display, Window window_id);

    void (*set_title)(Window window_id, char *value);
    void (*set_role)(Window window_id, char *value);
    void (*set_window_type)(Window window_id, char *value);
    void (*set_class)(Window window_id, char *value);
    void (*set_classname)(Window window_id, char *value);
    void (*set_opacity)(Window window_id, double value);

    void (*toggle_fixed_size)(Window window_id, Bool isEnabled, int width, int height);
    void (*toggle_sticky)(Window window_id, Bool isEnabled);
    void (*toggle_shade)(Window window_id, Bool isEnabled);
    void (*toggle_taskbar)(Window window_id, Bool isEnabled);
    void (*toggle_pager)(Window window_id, Bool isEnabled);
    void (*toggle_fullscreen)(Window window_id, Bool isEnabled);
    void (*toggle_above)(Window window_id, Bool isEnabled);
    void (*toggle_below)(Window window_id, Bool isEnabled);

    // XChangeProperty Function:
    //      https://tronche.com/gui/x/xlib/window-information/XChangeProperty.html
    // All Props:
    //      https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863921328

    void (*set_property)(Window window_id, char *property_name, char *atom_names[], int num_atoms, char *atom_type, int atom_format, HH_PROPERTY_LEN_MODE len_mode);

    // _NET_WM_STATE: https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863892896
    void (*send_event)(Window window_id, char *event_name, char *atom_names[], int num_atoms, HH_EVENT_MODE mode, Bool is_atom_raw);
} NSHHWindow;

extern NSHHWindow HHWindow;

#endif