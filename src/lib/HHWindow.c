#include "HHDisplay.h"
#include "HHWindow.h"

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

    if (0 != XGetWMNormalHints(display, window_id, hints, &supplied))
    {
        fprintf(stderr, "Error obtaining Window's XSizeHints: %ld\n", window_id);
        XCloseDisplay(display);
        exit(50);
    }

    return hints;
}

void undo_fixed_size(Window window_id)
{
    Display *display = HHDisplay.attach();
    XSizeHints *hints = get_normal_hints(display, window_id);

    hints->flags = ~(PMinSize | PMaxSize);

    // Set the WM_NORMAL_HINTS property on the specified window
    XSetWMNormalHints(display, window_id, hints);
    XFree(hints);

    HHDisplay.detach(display);
}

void set_fixed_size(Window window_id, char *width, char *height)
{
    Display *display = HHDisplay.attach();
    XSizeHints *hints = get_normal_hints(display, window_id);

    // Set the minimum and maximum size hints
    hints->flags = PMinSize | PMaxSize;
    hints->min_width = hints->max_width = atoi(width);
    hints->min_height = hints->max_height = atoi(height);

    // Set the WM_NORMAL_HINTS property on the specified window
    XSetWMNormalHints(display, window_id, hints);
    XFree(hints);

    HHDisplay.detach(display);
}

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

void send_event(Window window_id, char *event_name, char *atom_names[], int num_atoms, Bool add)
{
    Display *display = HHDisplay.attach();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window_id;
    xev.xclient.message_type = XInternAtom(display, event_name, False);
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = add ? 1 : 0;

    for (int i = 0; i < num_atoms; i += 2)
    {
        xev.xclient.data.l[1] = XInternAtom(display, atom_names[i], False);
        xev.xclient.data.l[2] = num_atoms < i ? 0 : XInternAtom(display, atom_names[i + 1], False);
        // https://specifications.freedesktop.org/wm-spec/1.3/ar01s05.html#:~:text=To%20change%20the%20state%20of%20a%20mapped%20window
        xev.xclient.data.l[3] = 1;
        xev.xclient.data.l[4] = 0;

        XSendEvent(display, window_id, False, NoEventMask, &xev);
    }

    HHDisplay.detach(display);
}

// Expose Namespace
NSHHWindow HHWindow = {
    .set_fixed_size = set_fixed_size,
    .undo_fixed_size = undo_fixed_size,
    .set_property = set_property,
    .send_event = send_event
    //
};