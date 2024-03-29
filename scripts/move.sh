#!/bin/bash

# Moves a Window with an ease function.
#
# Run this script, click a Window to begin!

OPT="scale=20;"

function assignvars {
  t=$1
  b=$2
  c=$3
  d=$4
}

function bounce_easeOut {
  assignvars "$1" "$2" "$3" "$4"

  t=$(echo "$OPT $t/$d" | bc -l)

  if [ "$(echo "$OPT $t < (1/2.75)" | bc -l)" == 1 ]
  then
    echo "$OPT $c*(7.5625*$t*$t) + $b" | bc -l
  elif [ "$(echo "$OPT $t < (2/2.75)" | bc -l)" == 1 ]
  then
    t=$(echo "$OPT $t-(1.5/2.75)" | bc -l)
    echo "$OPT $c*(7.5625*$t*$t + 0.75) + $b" | bc -l
  elif [ "$(echo "$OPT $t < (2.5/2.75)" | bc -l)" == 1 ]
  then
    t=$(echo "$OPT $t-(2.25/2.75)" | bc -l)
    echo "$OPT $c*(7.5625*$t*$t + 0.9375) + $b" | bc -l
  else
    t=$(echo "$OPT $t-(2.625/2.75)" | bc -l)
    echo "$OPT $c*(7.5625*$t*$t + 0.984375) + $b" | bc -l
  fi
}

easeMethod=bounce_easeOut

numSteps=200

endVal_x=${1:-0}
endVal_y=${2:-0}

window_id=$(xwininfo -int | grep -oP '(?<=Window id: )\d+')
pos_txt=$(xdotool getwindowgeometry "$window_id" | grep Position)
coords="${pos_txt#*: }"  # Remove everything before the colon and space
startVal_x="${coords%%,*}"    # Extract the substring before the comma
startVal_y="${coords#*,}"     # Extract the substring after the comma
startVal_y="${startVal_y%% (*}"        # Remove everything after the space

changeBy_x=$(echo "$endVal_x - $startVal_x" | bc -l)
changeBy_y=$(echo "$endVal_y - $startVal_y" | bc -l)

echo "Moving: $startVal_x -> $endVal_x ($changeBy_x), $startVal_y -> $endVal_y ($changeBy_y)"

for (( i=0; i<=numSteps; i++ ))
do
  easeVal_x="$($easeMethod $i "$startVal_x" "$changeBy_x" $numSteps)"
  easeVal_y="$($easeMethod $i "$startVal_y" "$changeBy_y" $numSteps)"

  # echo "Move: $easeVal_x, $easeVal_y"

  ./debug/edit_window --id "$window_id" --move="$easeVal_x, $easeVal_y" > /dev/null 2>&1 
done
