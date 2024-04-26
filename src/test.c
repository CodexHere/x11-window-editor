#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <stdio.h>

#include "./lib/HHDisplay.h"

Window User_Select_Window()
{
  Display *display = HHDisplay.attach();

  XEvent event;
  Window root = RootWindow(display, 0);
  Window target_win = None;
  int buttons = 0;

  /* Make the target cursor */
  Cursor cursor = XCreateFontCursor(display, XC_crosshair);

  /* Grab the pointer using target cursor, letting it room all over */
  int status = XGrabPointer(
      display,
      root,
      False,
      ButtonPressMask | ButtonReleaseMask,
      GrabModeSync,
      GrabModeAsync,
      root,
      cursor,
      CurrentTime
      //
  );

  if (status != GrabSuccess)
  {
    printf("Can't grab the mouse.");
    return 1;
  }

  /* Let the user select a window... */
  while ((target_win == None) || (buttons != 0)) // NOLINT
  {
    /* allow one more event */
    XAllowEvents(display, SyncPointer, CurrentTime);
    XWindowEvent(display, root, ButtonPressMask | ButtonReleaseMask, &event);

    switch (event.type)
    {
    case ButtonPress:
      if (target_win == None)
      {
        target_win = event.xbutton.subwindow; /* window selected */

        if (target_win == None)
        {
          target_win = root;
        }
      }
      buttons++;
      break;
    case ButtonRelease:
      if (buttons > 0) /* there may have been some down before we started */
      {
        buttons--;
      }
      break;
    default:
      break;
    }
  }

  XUngrabPointer(display, CurrentTime); /* Done with pointer */

  HHDisplay.detach(display);

  return (target_win);
}

int main()
{
  Window target_win = User_Select_Window();

  printf("Window Found: %i\n", (int)target_win);

  return 0;
}
