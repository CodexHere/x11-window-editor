#!/bin/sh

echo "Click on the window you want to monitor..."

window_id=$(xwininfo -int | grep -oP '(?<=Window id: )\d+')

echo "Watching Window: $window_id"

watch -n 1 xprop -id $window_id _NET_WM_NAME WM_CLASS WM_NORMAL_HINTS _NET_WM_STATE _NET_WM_WINDOW_TYPE
