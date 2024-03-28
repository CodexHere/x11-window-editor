#!/bin/sh


step() {
    local enterkey
    echo "------------\nStep: $1, Press any key to continue..."
    read enterkey
}

make
clear

echo "Click on the window you want to Test..."

window_id=$(xwininfo -int | grep -oP '(?<=Window id: )\d+')

echo "Testing Window: $window_id"

step "Maximize"
./debug/edit_window --id="$window_id" -m

step "Restore"
./debug/edit_window --id="$window_id" -r

step "Minimize"
./debug/edit_window --id="$window_id" -n

step "Restore"
./debug/edit_window --id="$window_id" -r

step "Toggle Fixed Size: 300x300"
./debug/edit_window --id="$window_id" -z=true --width=300 --height=300
step "Toggle Fixed Size: 500x500"
./debug/edit_window --id="$window_id" -z=true --width=500 --height=500
step "Toggle Fixed Size: Off"
./debug/edit_window --id="$window_id" -z

step "Toggle Always Above: On"
./debug/edit_window --id="$window_id" -a=true
step "Toggle Always Above: Off"
./debug/edit_window --id="$window_id" -a

step "Toggle Always Below: On"
./debug/edit_window --id="$window_id" -b=true
step "Toggle Always Below: Off"
./debug/edit_window --id="$window_id" -b

step "Toggle Shaded: On"
./debug/edit_window --id="$window_id" -s=true
step "Toggle Shaded: Off"
./debug/edit_window --id="$window_id" -s

step "Toggle Sticky: On"
./debug/edit_window --id="$window_id" -y=true
step "Toggle Sticky: Off"
./debug/edit_window --id="$window_id" -y

step "Toggle Taskbar: On"
./debug/edit_window --id="$window_id" -t=true
step "Toggle Taskbar: Off"
./debug/edit_window --id="$window_id" -t

step "Toggle Pager: On"
./debug/edit_window --id="$window_id" -p=true
step "Toggle Pager: Off"
./debug/edit_window --id="$window_id" -p

step "Toggle Fullscreen: On"
./debug/edit_window --id="$window_id" -f=true
step "Toggle Fullscreen: Off"
./debug/edit_window --id="$window_id" -f