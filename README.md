# X11Win Control

I wasn't able to find exactly what I needed to control X11 Windows exactly how I wanted.

There's `xdotool` which is great for a lot of things so definitely check that tool out, but not everything.

And `xprop` can do some of the other simpler stuff really well - but doesn't let you set [complex Atom Types](https://github.com/tmathmeyer/xprop/blob/master/xprop.c#L1749-L1755), such as [Window Properties](https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863921328) like [`_NET_WM_STATE`](https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863892896), or [`_NET_WM_WINDOW_TYPE`](https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46485863906176).

To be more accurate, it *does* let you set practically any property, but in most cases it isn't the correct approach by using [`XChangeProperty`](https://tronche.com/gui/x/xlib/window-information/XChangeProperty.html). While this does allow setting properties such as `_NET_WM_STATE`, in many cases you need to send an [`XEvent`](https://tronche.com/gui/x/xlib/events/client-communication/client-message.html) for the Window Manager to actually care about your values you're setting.

So that's why I built this simple command line tool in C - to do the quick and dirty editing these tools leave out!

## Requirements

```sh
sudo apt install libx11-dev
```

## Building

```sh
make
```

## Install

```sh
make install
# or
make install DIR_INSTALL=../output
```

## Usage

```sh
${DIR_INSTALL}/edit_window
```

Review the `Help` output from the application.

## Useful Links

* [Extended Window Manager Hints Documentation](https://specifications.freedesktop.org/wm-spec/latest/)
* [`_NET_WM_WINDOW_TYPE`s](https://specifications.freedesktop.org/wm-spec/latest/ar01s05.html#idm44882398096864)
* [`_NET_WM_STATE`s](https://specifications.freedesktop.org/wm-spec/latest/ar01s05.html#idm44882398084176)
