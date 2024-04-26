#!/bin/bash

window_id=$(xwininfo -int | grep -oP '(?<=Window id: )\d+')

for i in $(seq 0 0.005 1); do
    ./debug/edit_window --id="$window_id" --set-opacity="$i" > /dev/null 2>&1 
    sleep 0.001 
done