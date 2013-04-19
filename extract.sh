#!/bin/bash
i=0
depth="depth.png"
color="color"

for file in view*
do
	cd $file
	for k in *.png
	do
		if [ "$k" = "$depth" ]
		then
		echo $k	

	mv "${k}" "/home/piyush/Desktop/Clap/$(printf "depth%04d.png" $i)"
		fi

	done
	i=$((i+1))
	cd ..
done





