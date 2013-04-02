#!/bin/bash
#ls $1 | grep "\.wmv"
IFS=$'\n'
video=( $(ls $1 | grep "\.wmv") )
video_trunc=( $(ls $1 | grep "\.wmv" | sed 's/\.wmv//g;s/\ /_/g;s/(//g;s/)//g') )
arraysize=$(ls $1 | grep "\.wmv" | wc -l)

#g++ -o /home/randylab/imager/bin/imager /home/randylab/imager/src/imager.cpp -lpthread -lX11 -I/usr/include/ImageMagick -lMagick++ -lMagickWand -lMagickCore -Wall -Wextra -g
#g++ -o /home/randylab/imager/bin/crop /home/randylab/imager/src/crop.cpp -L/opt/lib -lpthread -lX11 -Wall -Wextra -g

for ((a=0; a < arraysize ; a++)) do
	ffmpeg -i "$1/${video[$a]}" "$1/${video_trunc[$a]}".jpg
	mkdir $1/${video_trunc[$a]}
	/home/randylab/imager/bin/crop "$1/${video_trunc[$a]}".jpg
	rm "$1/${video_trunc[$a]}".jpg
done

for ((b=0; b < arraysize ; b++)) do
	c=( $( head -$((4+4*$b)) $1/crop.txt | tail -4 ) )
	echo ${c[0]} ${c[1]} ${c[2]} ${c[3]}
	#ffmpeg -i "${video[$b]}" -r 1 -cropleft ${c[0]} -cropright ${c[1]} -croptop ${c[2]} -cropbottom ${c[3]} "${video_trunc[$b]}"/image%d.jpg
	#/home/randylab/imager/bin/imager ${video_trunc[$b]}&
	#t=$!
	#sleep 300
	#kill $!
done

rm crop.txt






#-croptop 45 -cropbottom 85 -cropleft 73 -cropright 180
#known crop settings for USB Video Device (2011-10-14 10h-20m-55s)



#./imager ${video_trunc[0]} ${video_trunc[1]} ${video_trunc[2]}
#or ./test "$video"


#g++ -o imager test1.cpp -lpthread -lX11 -Wall -Wextra -g
#./imager
