#!/bin/sh

# Monitors a Window for useful Properties.
#
# Run this script, click a Window to begin!

echo "Click on the window you want to monitor..."

window_id=$(xwininfo -int | grep -oP '(?<=Window id: )\d+')

echo "Watching Window: $window_id"

watch -n 1 xprop -id $window_id _NET_WM_NAME WM_CLASS WM_WINDOW_ROLE _NET_WM_WINDOW_OPACITY WM_NORMAL_HINTS _NET_WM_STATE _NET_WM_WINDOW_TYPE
