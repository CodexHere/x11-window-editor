#include "HHDisplay.h"
#include "HHWindow.h"

////////////////////////////////////////////////////////////
// Helper Methods
////////////////////////////////////////////////////////////

void set_property(Window window_id, char *property_name, char *atom_names[], int num_atoms)
{
    Display *display = HHDisplay.attach();

    // Get the atoms for the allowed actions
    Atom actions[num_atoms];
    int numActions = 0;

    for (int i = 0; i < num_atoms; i++)
    {
        Atom action = XInternAtom(display, atom_names[i], False);
        if (action != None)
            actions[numActions++] = action;
    }

    // Change the allowed actions property
    Atom propertyAtom = XInternAtom(display, property_name, False);

    XChangeProperty(
        display,
        window_id,
        propertyAtom,
        XA_ATOM,
        32,
        PropModeReplace,
        (unsigned char *)actions,
        numActions);

    HHDisplay.detach(display);
}

void send_event(Window window_id, char *event_name, char *atom_names[], int num_atoms, long mode)
{
    Display *display = HHDisplay.attach();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window_id;
    xev.xclient.message_type = XInternAtom(display, event_name, False);
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = mode;

    for (int i = 0; i < num_atoms; i += 2)
    {
        xev.xclient.data.l[1] = XInternAtom(display, atom_names[i], False);
        xev.xclient.data.l[2] = i + 1 >= num_atoms ? 0 : XInternAtom(display, atom_names[i + 1], False);
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

void toggleFixedSize(Window window_id, Bool enable, char *width, char *height)
{
    Display *display = HHDisplay.attach();
    XSizeHints *hints = get_normal_hints(display, window_id);

    if (enable)
    {
        // Set the minimum and maximum size hints
        hints->flags = PMinSize | PMaxSize;
        hints->min_width = hints->max_width = atoi(width);
        hints->min_height = hints->max_height = atoi(height);
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
    send_event(window_id, "WM_CHANGE_STATE", atoms, 1, IconicState);
}

void maximize(Window window_id)
{
    char *atoms[] = {
        "_NET_WM_STATE_MAXIMIZED_HORZ",
        "_NET_WM_STATE_MAXIMIZED_VERT"};
    send_event(window_id, "_NET_WM_STATE", atoms, 2, HH_EVENT_MODE_ADD);
}

void restore(Window window_id)
{
    // char *atoms1[] = {"_NET_WM_STATE_STICKY"};
    char *atoms2[] = {
        "_NET_WM_STATE_MAXIMIZED_HORZ",
        "_NET_WM_STATE_MAXIMIZED_VERT",
        "_NET_WM_STATE_HIDDEN",
        "_NET_WM_STATE_STICKY"
        //
    };

    // send_event(window_id, "WM_STATE_CHANGE", atoms1, 1, HH_EVENT_MODE_REMOVE);
    send_event(window_id, "_NET_WM_STATE", atoms2, 4, HH_EVENT_MODE_REMOVE);
}

void toggleAbove(Window window_id, long mode)
{
    char *atoms[] = {"_NET_WM_STATE_ABOVE"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, mode);
}

void toggleBelow(Window window_id, long mode)
{
    char *atoms[] = {"_NET_WM_STATE_BELOW"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, mode);
}

void toggleFullscreen(Window window_id, long mode)
{
    char *atoms[] = {"_NET_WM_STATE_FULLSCREEN"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, mode);
}

void togglePager(Window window_id, long mode)
{
    char *atoms[] = {"_NET_WM_STATE_SKIP_PAGER"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, mode);
}

void toggleShade(Window window_id, long mode)
{
    char *atoms[] = {"_NET_WM_STATE_SHADED"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, mode);
}

void toggleSticky(Window window_id, long mode)
{
    char *atoms[] = {"_NET_WM_STATE_STICKY"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, mode);
}

void toggleTaskbar(Window window_id, long mode)
{
    char *atoms[] = {"_NET_WM_STATE_SKIP_TASKBAR"};
    send_event(window_id, "_NET_WM_STATE", atoms, 1, mode);
}

////////////////////////////////////////////////////////////
// Namespace Definition
////////////////////////////////////////////////////////////

NSHHWindow HHWindow = {

    .minimize = minimize,
    .maximize = maximize,

    .toggleAbove = toggleAbove,
    .toggleBelow = toggleBelow,
    .toggleFixedSize = toggleFixedSize,
    .toggleFullscreen = toggleFullscreen,
    .togglePager = togglePager,
    .toggleShade = toggleShade,
    .toggleSticky = toggleSticky,
    .toggleTaskbar = toggleTaskbar,

    .restore = restore,

    .set_property = set_property,
    .send_event = send_event
    //
};