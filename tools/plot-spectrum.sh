#!/bin/bash
# Joker TV blind scan spectrum draw using gnuplot
# (c) Abylay Ospan <aospan@jokersys.com>, 2018
# LICENSE: GPLv2

usage() { echo "Generate spectrum based on Joker TV blind scan results
        Usage:
            $0 -p power.csv -l locked.csv -o out.png -y voltage
                -p file produced by joker-tv --blind-power
                -l file produced by joker-tv --blind-out
                -o output PNG filename with spectrum
                -y voltage. 13 or 18
    " 1>&2; exit 1; }

while getopts "y:o:l:p:" o; do
    case "${o}" in
        o)
            out=${OPTARG}
            ;;
        p)
            power=${OPTARG}
            ;;
        l)
            locked=${OPTARG}
            ;;
        y)
            voltage=${OPTARG}
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [ -z "${out}" ] || [ -z "${power}" ] || [ -z "${locked}" ] || [ -z "${voltage}" ]; then
    usage
fi

if [ "${voltage}" = "13" ]; then
    colour="red"
else
    colour="blue"
fi

#get LOCKed transponders
#format example: "11889","13v V(R)","DVB-S2","7198"
while IFS=, read -r freq pol standard ksym
do
    freq=${freq//\"}
    standard=${standard//\"}
    ksym=${ksym//\"}
    pol=${pol//\"}
    if [ "$freq" -eq "$freq" ] 2>/dev/null && [[ "$pol" =~ "${voltage}".+ ]]; then
        labels+="set arrow from $freq, graph 0 to $freq, graph 1 nohead"
        labels+=$'\n'
        labels+="set label \"$standard $freq MHz $pol $ksym ksym\" at $freq-5,graph 0 rotate"
        labels+=$'\n'
    fi
done < $locked

#draw final spectrum
echo "Generating spectrum ..."
gnuplot -persist <<-EOFMarker
    set grid xtics
    set xtics 40 format "%.1f" scale 2 rotate
    set grid x2tics
    set autoscale fix
    set x2tics 10 format "%.1f" scale 1 offset 0,graph 0 rotate font "sans,6"
    $labels
    set terminal png size 1600,1200
    set output '$out'
    plot '$power' lt rgb "$colour" with lines
EOFMarker
