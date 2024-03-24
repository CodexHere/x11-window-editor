#include "HHDisplay.h"

Display *current_display;

// Attach to the X Server and get the Display.
Display *attach()
{
    // Return cached instance
    if (NULL != current_display)
    {
        return current_display;
    }

    // Not cached... Attach!
    Display *display = XOpenDisplay(NULL);

    if (display == NULL)
    {
        fprintf(stderr, "Error opening display\n");
        exit(99);
    }

    return display;
}

// Flush and close the Display.
void detach(Display *display)
{
    XFlush(display);
    XCloseDisplay(display);
}

// Expose Namespace
NSHHDisplay HHDisplay = {
    .attach = attach,
    .detach = detach
    //
};