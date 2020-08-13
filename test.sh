#!/bin/sh
PROGRAM=${PWD##*/}

WIDTH=512
HEIGHT=512
COLOR=(255 0 0)
BACKGROUND=(255 255 255)
OFFSCREEN=1

rm log.zip

for OPACITY in 0.2 0.4 0.6 0.8 1.0
do
    for NPOLYGONS in 200 400 600 800 1000
    do
	for NREPEATS in 1 10 50 100
	do
	    for NPEELS in 1 10 50 100
	    do
		echo "Opacity: $OPACITY"
		echo "Number of polygons: $NPOLYGONS"
		echo "Number of repeats: $NREPEATS"
		echo "Number of peels: $NPEELS"
		echo ""
		./$PROGRAM -width $WIDTH -height $HEIGHT -nrepeats $NREPEATS -npeels $NPEELS -npolygons $NPOLYGONS -opacity $OPACITY -color ${COLOR[0]} ${COLOR[1]} ${COLOR[2]} -background ${BACKGROUND[0]} ${BACKGROUND[1]} ${BACKGROUND[2]} -offscreen $OFFSCREEN -seed 0
	    done
	done
    done
done

zip log.zip log.csv *.bmp
rm *.bmp
rm log.csv

