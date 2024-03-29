#!/bin/bash

# Resizes a Window with an ease function.
#
# Run this script, click a Window to begin!

PI=3.14
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

function elastic_easeOut {
        assignvars "$1" "$2" "$3" "$4" "$5" "$6"
        a=$s
        TMP=$(echo "$OPT $t/$d" | bc -l)
        if [ -z "$a" ]
        then
          a=$c
        fi
        if [ -z "$p" ]
        then
          p=$(echo "$OPT $d * 0.3" | bc -l)
        fi
      #-------------------------------------------------
        if [ "$(echo "$OPT $t == 0" | bc -l)" == 1 ]
        then
          echo "$b"
        elif [ "$(echo "$OPT $TMP == 1" | bc -l)" == 1 ]
        then
          echo "$OPT $b+$c" | bc -l
        else
          if [ "$(echo "$OPT $a < ${c#-}" | bc -l)" == 1 ]
          then
            s=$(echo "$OPT $p / 4" | bc -l)
          else
            cda=$(echo "$OPT $c/$a" | bc -l)
            ASIN=$(asin "$cda")
            s=$(echo "$OPT $p/(2*$PI) * $ASIN " | bc -l)
          fi
        echo "$OPT $a * e(l(2) * (-10*$TMP)) * s( ($TMP*$d-$s)*(2*$PI)/$p ) +$c + $b" | bc -l
        fi
}

function asin {
  x=$1
  if [ "$(echo "$OPT $x == 1" | bc -l)" == 1 ]
  then
    echo "$OPT $PI/2" | bc -l
  elif [ "$(echo "$OPT $x == -1" | bc -l)" == 1 ]
  then
    echo "$OPT -1*$PI/2" | bc -l
  else
    echo "$OPT ( a($x / sqrt(1-($x ^ 2))  ) )" | bc -l
  fi
}



easeMethod=elastic_easeOut

numSteps=100

endVal_width=${1:-500}
endVal_height=${2:-500}

window_id=$(xwininfo -int | grep -oP '(?<=Window id: )\d+')
geom_txt=$(xdotool getwindowgeometry "$window_id" | grep Geometry)
dimensions=${geom_txt#*: }  # Remove everything before the colon and space
width=${dimensions%x*}  # Remove everything after the 'x' character
height=${dimensions#*x}  # Remove everything before the 'x' character

changeBy_width=$(echo "$endVal_width - $width" | bc -l)
changeBy_height=$(echo "$endVal_height - $height" | bc -l)

echo "Resizing: $width -> $endVal_width ($changeBy_width), $height -> $endVal_height ($changeBy_height)"

for (( i=0; i<=numSteps; i++ ))
do
  easeVal_width="$($easeMethod $i "$width" "$changeBy_width" $numSteps)"
  easeVal_height="$($easeMethod $i "$height" "$changeBy_height" $numSteps)"

  # echo "Resize: $easeVal_width, $easeVal_height"

  ./debug/edit_window --id "$window_id" --width "$easeVal_width" --height "$easeVal_height" --size > /dev/null 2>&1 
done
