# X11Win Control

I wasn't able to find exactly what I needed to control X11 Windows exactly how I wanted.

There's `xdotool` which is great for a lot of things so definitely check that tool out, but not everything.

And `xprop` can do some of the other simpler stuff really well - but doesn't let you set [complex Atom Types](https://github.com/tmathmeyer/xprop/blob/master/xprop.c#L1749-L1755), such as [Window Properties](https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863921328) like [`_NET_WM_STATE`](https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863892896), or [`_NET_WM_WINDOW_TYPE`](https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863906176).

To be more accurate, it *does* let you set practically any property, but in most cases it isn't the correct approach by using [`XChangeProperty`](https://tronche.com/gui/x/xlib/window-information/XChangeProperty.html). While this does allow setting properties such as `_NET_WM_STATE`, in many cases you need to send an [`XEvent`](https://tronche.com/gui/x/xlib/events/client-communication/client-message.html) for the Window Manager to actually care about your values you're setting.

So that's why I built this simple command line tool in C - to do the quick and dirty editing these tools leave out!

## Requirements

```sh
sudo apt install -y libx11-dev

# for dev

sudo apt install -y inotify-tools
```

## Building

A simple, but flexible, `Makefile` is included with the project:

```sh
make
```

## Developing

A helpful script is included to rebuild the binaries when a source file is changed:

```sh
./scripts/dev.sh
```

### Other Helpful Scripts

A couple other scripts are included which can be helpful to get information about a window, as well as run a window through the gamut.

#### Monitor

Monitor a window's status with `xprop`:

```sh
./scripts/monitor.sh
```

You'll click a window, and the information that's pertinent will be updated every 1s.

#### Testers

> With all Testers (unless stated otherwise), you'll click a window, and the script will walk you through the test.

Put a Window through the gamut with the test script:

```sh
./scripts/test.sh
```

Perform an advanced Move with the move script:

```sh
./scripts/move.sh
```

Perform an advanced Resize with the resize script:

```sh
./scripts/resize.sh
```

## Install

```sh
make install
# or
make install DIR_INSTALL=../output
```

> Default `DIR_INSTALL` is set to `./release`.

## Usage

```sh
${DIR_INSTALL}/edit_window
```

Review the `Help` output from the application.

## Useful Links

* [Extended Window Manager Hints Documentation](https://specifications.freedesktop.org/wm-spec/latest/)
* [`_NET_WM_WINDOW_TYPE`s](https://specifications.freedesktop.org/wm-spec/latest/ar01s05.html#idm44882398096864)
* [`_NET_WM_STATE`s](https://specifications.freedesktop.org/wm-spec/latest/ar01s05.html#idm44882398084176)

## TODO

Backfill most, if not all niceties of `xdotool`/`xprop`/`wmctrl` needed for my Workspace Manager:
    * Add ability to select window if one isn't provided!
        * https://github.com/tmathmeyer/xprop/blob/master/dsimple.c#L274-L283
    * Raise
    * Move to Monitor?
    * Move to Desktop?
    * _NET_WM_STRUT Support?
    * Motifs?
        * _MOTIF_WM_HINTS
        * https://stackoverflow.com/a/5186291
        * https://codebrowser.dev/gtk/gtk/gdk/x11/MwmUtil.h.html
        * https://gist.github.com/muktupavels/d03bb14ea6042b779df89b4c87df975d
    * Test helper scripts need window_id and other params injected

Ideas:
    * Force window to systray
        * Captures an apps icon, and unmaps the window, and creates a systray icon to tuck the app away indefinitely. Will need the ability to restore the app, maybe just simply clicking the icon can restore it?
        * I think `initial_state=IconicState` from https://tronche.com/gui/x/xlib/ICC/client-to-window-manager/wm-hints.html#XWMHints
        * https://specifications.freedesktop.org/systemtray-spec/systemtray-spec-0.2.html