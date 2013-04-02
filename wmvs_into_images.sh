#!/bin/bash
#ls $1 | grep "\.wmv"
IFS=$'\n'
video=( $(ls $1 | grep "\.wmv") )
video_trunc=( $(ls $1 | grep "\.wmv" | sed 's/\.wmv//g;s/\ /_/g;s/(//g;s/)//g') )
arraysize=$(ls $1 | grep "\.wmv" | wc -l)

#g++ -o /home/randylab/imager/bin/imager /home/randylab/imager/src/imager.cpp -lpthread -lX11 -I/usr/include/ImageMagick -lMagick++ -lMagickWand -lMagickCore -Wall -Wextra -g
#g++ -std=c++11 -o /home/randylab/imager/bin/crop /home/randylab/imager/src/crop.cpp -L/opt/lib -lpthread -lX11 -Wall -Wextra -g

for ((a=0; a < arraysize ; a++)) do
	#ffmpeg -i "$1/${video[$a]}" "$1/${video_trunc[$a]}".jpg
	#mkdir $1/${video_trunc[$a]}
	#/home/randylab/imager/bin/crop "$1/${video_trunc[$a]}".jpg
	rm "$1/${video_trunc[$a]}".jpg
done

for ((b=0; b < arraysize ; b++)) do
	c=( $( head -$((1+1*$b)) $1/crop.txt | tail -4 ) )
	echo $c
	ffmpeg -i "$1/${video[$b]}" -r 1 -vf crop=$c "$1/${video_trunc[$b]}"/image%d.jpg
	#/home/randylab/imager/bin/imager ${video_trunc[$b]}&
	#t=$!
	#sleep 300
	#kill $!
done

#rm $1/crop.txt