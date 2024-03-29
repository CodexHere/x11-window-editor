#include "HHDisplay.h"
#include "HHWindow.h"

////////////////////////////////////////////////////////////
// Helper Methods
////////////////////////////////////////////////////////////

void set_property(Window window_id, char *property_name, char *atom_names[], int num_atoms, char *atom_property_type, int atom_format, Bool is_atom_raw)
{
    Display *display = HHDisplay.attach();

    // Store the ATOMs for the incoming Atom names
    Atom atoms[num_atoms];
    unsigned char *value;
    int value_len;
    int num_valid_atoms = 0;

    for (int i = 0; i < num_atoms; i++)
    {
        Atom atom = XInternAtom(display, atom_names[i], False);
        if (atom != None)
            atoms[num_valid_atoms++] = atom;
    }

    // Determine the `value` and `value_len` based on whether we expect a simple string set or not
    // Value is either the first "atom" as a simple string, or all atoms combined
    value = is_atom_raw ? (unsigned char *)atom_names[0] : (unsigned char *)atoms;
    value_len = is_atom_raw ? (int)strlen((char *)value) : num_valid_atoms;

    if (0 == strlen((char *)value))
    {
        return;
    }

    // Build ATOM for the Property we want to change
    Atom atom_property = XInternAtom(display, property_name, False);
    Atom atom_atom_property_type = XInternAtom(display, atom_property_type, False);

    // printf(
    //     "Setting Property (%s): %s(%ld) to %s (len=%i) [%s=%ld]",
    //     expectingAString ? "Simple" : "Complex",
    //     property_name,
    //     atom_property,
    //     value,
    //     value_len,
    //     atom_property_type,
    //     atom_atom_property_type);

    XChangeProperty(
        display,
        window_id,
        atom_property,
        atom_atom_property_type,
        atom_format,
        PropModeReplace,
        value,
        value_len);

    HHDisplay.detach(display);
}

void send_event(Window window_id, char *event_name, char *atom_names[], int num_atoms, HH_EVENT_MODE eventMode, Bool is_atom_raw)
{
    Display *display = HHDisplay.attach();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window_id;
    xev.xclient.message_type = XInternAtom(display, event_name, False);
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = eventMode;

    for (int i = 0; i < num_atoms; i += 2)
    {
        if (False == is_atom_raw)
        {

            xev.xclient.data.l[1] = (long)XInternAtom(display, atom_names[i], False);
            xev.xclient.data.l[2] = i + 1 >= num_atoms ? 0 : (long)XInternAtom(display, atom_names[i + 1], False);
        }
        else
        {
            xev.xclient.data.l[1] = strtol(atom_names[i], NULL, 0);
            xev.xclient.data.l[2] = i + 1 >= num_atoms ? 0 : strtol(atom_names[i + 1], NULL, 0);
        }
        // https://specifications.freedesktop.org/wm-spec/1.3/ar01s05.html#:~:text=To%20change%20the%20state%20of%20a%20mapped%20window
        xev.xclient.data.l[3] = 0;
        xev.xclient.data.l[4] = 0;

        XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
    }

    HHDisplay.detach(display);
}

////////////////////////////////////////////////////////////
// Normal Hints
////////////////////////////////////////////////////////////

XSizeHints *get_normal_hints(Display *display, Window window_id)
{
    XSizeHints *hints = XAllocSizeHints();
    long supplied;

    if (hints == NULL)
    {
        fprintf(stderr, "Error allocating XSizeHints\n");
        XCloseDisplay(display);
        exit(99);
    }

    if (0 == XGetWMNormalHints(display, window_id, hints, &supplied))
    {
        fprintf(stderr, "Error obtaining Window's XSizeHints: %ld\n", window_id);
        XCloseDisplay(display);
        exit(50);
    }

    return hints;
}

void toggle_fixed_size(Window window_id, Bool enable, int width, int height)
{
    Display *display = HHDisplay.attach();
    XSizeHints *hints = get_normal_hints(display, window_id);

    if (enable)
    {
        // Set the minimum and maximum size hints
        hints->flags = PMinSize | PMaxSize;
        hints->min_width = hints->max_width = width;
        hints->min_height = hints->max_height = height;
    }
    else
    {
        hints->flags = ~(PMinSize | PMaxSize);
    }

    // Set the WM_NORMAL_HINTS property on the specified window
    XSetWMNormalHints(display, window_id, hints);
    XFree(hints);

    HHDisplay.detach(display);
}

////////////////////////////////////////////////////////////
// State
////////////////////////////////////////////////////////////

void minimize(Window window_id)
{
    char *atoms[] = {"_NET_WM_STATE_HIDDEN"};
    send_event(window_id, "WM_CHANGE_STATE", atoms, 1, HH_EVENT_MODE_MINIMIZE, False);
}

void maximize(Window window_id)
{
    char *atoms[] = {
        "_NET_WM_STATE_MAXIMIZED_HORZ",
        "_NET_WM_STATE_MAXIMIZED_VERT"};
    send_event(window_id, "_NET_WM_STATE", atoms, 2, HH_EVENT_MODE_ADD, False);
}

void set_title(Window window_id, char *value)
{
    set_property(window_id, "_NET_WM_NAME", (char *[]){value}, 1, "UTF8_STRING", 8, True);
}

void set_role(Window window_id, char *value)
{
    set_property(window_id, "WM_WINDOW_ROLE", (char *[]){value}, 1, "STRING", 8, True);
}

void set_window_type(Window window_id, char *value)
{
    set_property(window_id, "_NET_WM_WINDOW_TYPE", (char *[]){value}, 1, "ATOM", 32, False);
}

void raise(Window window_id)
{
    // https://specifications.freedesktop.org/wm-spec/1.3/ar01s03.html#:~:text=_NET_ACTIVE_WINDOW%2C%20WINDOW/32

    send_event(window_id, "_NET_ACTIVE_WINDOW", (char *[]){"0"}, 1, HH_EVENT_MODE_ADD, True);
}

void restore(Window window_id)
{
    char *atoms[] = {
        "_NET_WM_STATE_MAXIMIZED_HORZ",
        "_NET_WM_STATE_MAXIMIZED_VERT",
        "_NET_WM_STATE_HIDDEN",
        "_NET_WM_STATE_STICKY"
        //
    };

    send_event(window_id, "_NET_WM_STATE", atoms, 4, HH_EVENT_MODE_REMOVE, False);
}

void toggle_above(Window window_id, Bool isEnabled)
{
    char *atoms[] = {"_NET_WM_STATE_ABOVE"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, isEnabled ? HH_EVENT_MODE_ADD : HH_EVENT_MODE_REMOVE, False);
}

void toggle_below(Window window_id, Bool isEnabled)
{
    char *atoms[] = {"_NET_WM_STATE_BELOW"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, isEnabled ? HH_EVENT_MODE_ADD : HH_EVENT_MODE_REMOVE, False);
}

void toggle_fullscreen(Window window_id, Bool isEnabled)
{
    char *atoms[] = {"_NET_WM_STATE_FULLSCREEN"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, isEnabled ? HH_EVENT_MODE_ADD : HH_EVENT_MODE_REMOVE, False);
}

void toggle_pager(Window window_id, Bool isEnabled)
{
    char *atoms[] = {"_NET_WM_STATE_SKIP_PAGER"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, isEnabled ? HH_EVENT_MODE_ADD : HH_EVENT_MODE_REMOVE, False);
}

void toggle_shade(Window window_id, Bool isEnabled)
{
    char *atoms[] = {"_NET_WM_STATE_SHADED"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, isEnabled ? HH_EVENT_MODE_ADD : HH_EVENT_MODE_REMOVE, False);
}

void toggle_sticky(Window window_id, Bool isEnabled)
{
    char *atoms[] = {"_NET_WM_STATE_STICKY"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, isEnabled ? HH_EVENT_MODE_ADD : HH_EVENT_MODE_REMOVE, False);
}

void toggle_taskbar(Window window_id, Bool isEnabled)
{
    char *atoms[] = {"_NET_WM_STATE_SKIP_TASKBAR"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, isEnabled ? HH_EVENT_MODE_ADD : HH_EVENT_MODE_REMOVE, False);
}

////////////////////////////////////////////////////////////
// Namespace Definition
////////////////////////////////////////////////////////////

NSHHWindow HHWindow = {

    .minimize = minimize,
    .maximize = maximize,
    .raise = raise,
    .set_title = set_title,
    .set_role = set_role,
    .set_window_type = set_window_type,

    .toggle_above = toggle_above,
    .toggle_below = toggle_below,
    .toggle_fixed_size = toggle_fixed_size,
    .toggle_fullscreen = toggle_fullscreen,
    .toggle_pager = toggle_pager,
    .toggle_shade = toggle_shade,
    .toggle_sticky = toggle_sticky,
    .toggle_taskbar = toggle_taskbar,

    .restore = restore,

    .set_property = set_property,
    .send_event = send_event
    //
};