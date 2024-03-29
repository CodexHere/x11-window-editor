#!/bin/sh

# Starts a dev session, recompiling the project on file changes.

PATH_SRC=src

CMD_WAIT="inotifywait -q -r -e modify"

while $CMD_WAIT $PATH_SRC; do
    make;
done
