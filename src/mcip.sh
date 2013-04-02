#!/bin/bash

#echo "Enter the number of the protocol to run."
#echo "1 - Random"
#echo "2 - Small increase"
#echo "3 - Large increase"
#echo "4 - Read from manual.txt"

#read choice

echo "Enter the collection time in seconds (1 hour = 3600 seconds)"

read coll_time

this_date=$(date +%F)
mkdir "$this_date" "$this_date/7va" "$this_date/8va" "$this_date/9va" "$this_date/10va" "$this_date/11va" "$this_date/4va" "$this_date/4_5va" "$this_date/5va" "$this_date/5_5va" "$this_date/6va" "$this_date/7vb" "$this_date/8vb" "$this_date/9vb" "$this_date/10vb" "$this_date/11vb" "$this_date/4vb" "$this_date/4_5vb" "$this_date/5vb" "$this_date/5_5vb" "$this_date/6vb" "$this_date/7vc" "$this_date/8vc" "$this_date/9vc" "$this_date/10vc" "$this_date/11vc" "$this_date/4vc" "$this_date/4_5vc" "$this_date/5vc" "$this_date/5_5vc" "$this_date/6vc"

g++ -o imager src/imager.cpp -lpthread -lX11 -Wall -Wextra -g
g++ -o crop src/crop.cpp -lpthread -lX11 -Wall -Wextra -g

ffmpeg -f video4linux2 -s 640x480 -i /dev/video0 $this_date/crop.jpg

./crop $this_date/crop.jpg

values_array=( $( head -4 $this_date/crop.txt ) )
echo ${values_array[0]} ${values_array[1]} ${values_array[2]} ${values_array[3]}

choice=$(($RANDOM % 3 + 1))

if [ $choice -eq 1 ]
then
	speeds=$( for i in `cat protocols/random.txt`; do echo "$RANDOM $i"; done | sort | sed -r 's/^[0-9]+ //' | awk '{print $0; if(NR % 1 == 0) print "0"}' | sed '$d' )
	for i in ${speeds}
	do
		if [ "$i" -eq "0" ]
		then
			sudo ./motor/close
			sleep 2
			sudo ./motor/11v
			sleep 2
			sudo ./motor/close
			sleep 2
			sudo ./motor/11v
			sleep 2
			sudo ./motor/close
			sleep 2
			sudo ./motor/11v
			sleep 2
			sudo ./motor/close
			continue
		fi
		sudo ./motor/"$i"v
		echo "$i"v >> $this_date/order.txt
		date | cat >> $this_date/order.txt
		ffmpeg -f video4linux2 -i /dev/video0 -t $coll_time -r .5 -s 640x480 $this_date/"$i"va_video.wmv
		ffmpeg -i $this_date/"$i"va_video.wmv -t $coll_time -r .5 -s 640x480 -croptop ${values_array[0]} -cropbottom ${values_array[1]} -cropleft ${values_array[2]} -cropright ${values_array[3]} $this_date/"$i"va/image%d.jpg
		./imager $this_date/"$i"va &
		sudo ./motor/close
		sleep 2
		sudo ./motor/11v
		sleep 2
		sudo ./motor/close
		sleep 2
		sudo ./motor/11v
		sleep 2
		sudo ./motor/close
		sleep 2
		sudo ./motor/11v
		sleep 2
		sudo ./motor/close
	done

	speeds=$( cat protocols/sm_incr.txt )
	for i in ${speeds}
	do
		#if [ "$i" -eq "0" ]
		#then
		#	sudo ./motor/close
		#	sleep 1200  
		#	continue
		#fi
		sudo ./motor/"$i"v
		echo "$i"v >> $this_date/order.txt
		date | cat >> $this_date/order.txt
		ffmpeg -f video4linux2 -i /dev/video0 -t $coll_time -r .5 -s 640x480 $this_date/"$i"vb_video.wmv
		ffmpeg -i $this_date/"$i"vb_video.wmv -t $coll_time -r .5 -s 640x480 -croptop ${values_array[0]} -cropbottom ${values_array[1]} -cropleft ${values_array[2]} -cropright ${values_array[3]} $this_date/"$i"vb/image%d.jpg
		./imager $this_date/"$i"vb &
		sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
	done

	speeds=$( cat protocols/lg_incr.txt )
	for i in ${speeds}
	do
		#if [ "$i" -eq "0" ]
		#then
		#	sudo ./motor/close
		#	sleep 1200  
		#	continue
		#fi
		sudo ./motor/"$i"v
		echo "$i"v >> $this_date/order.txt
		date | cat >> $this_date/order.txt
		ffmpeg -f video4linux2 -i /dev/video0 -t $coll_time -r .5 -s 640x480 $this_date/"$i"vc_video.wmv
		ffmpeg -i $this_date/"$i"vc_video.wmv -t $coll_time -r .5 -s 640x480 -croptop ${values_array[0]} -cropbottom ${values_array[1]} -cropleft ${values_array[2]} -cropright ${values_array[3]} $this_date/"$i"vc/image%d.jpg
		./imager $this_date/"$i"vc &
		sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
	done
fi

if [ $choice -eq 2 ]
then
	speeds=$( cat protocols/sm_incr.txt )
	for i in ${speeds}
	do
		#if [ "$i" -eq "0" ]
		#then
		#	sudo ./motor/close
		#	sleep 1200  
		#	continue
		#fi
		sudo ./motor/"$i"v
		echo "$i"v >> $this_date/order.txt
		date | cat >> $this_date/order.txt
		ffmpeg -f video4linux2 -i /dev/video0 -t $coll_time -r .5 -s 640x480 $this_date/"$i"vb_video.wmv
		ffmpeg -i $this_date/"$i"vb_video.wmv -t $coll_time -r .5 -s 640x480 -croptop ${values_array[0]} -cropbottom ${values_array[1]} -cropleft ${values_array[2]} -cropright ${values_array[3]} $this_date/"$i"vb/image%d.jpg
		./imager $this_date/"$i"vb &
		sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
	done

	speeds=$( cat protocols/lg_incr.txt )
	for i in ${speeds}
	do
		#if [ "$i" -eq "0" ]
		#then
		#	sudo ./motor/close
		#	sleep 1200  
		#	continue
		#fi
		sudo ./motor/"$i"v
		echo "$i"v >> $this_date/order.txt
		date | cat >> $this_date/order.txt
		ffmpeg -f video4linux2 -i /dev/video0 -t $coll_time -r .5 -s 640x480 $this_date/"$i"vc_video.wmv
		ffmpeg -i $this_date/"$i"vc_video.wmv -t $coll_time -r .5 -s 640x480 -croptop ${values_array[0]} -cropbottom ${values_array[1]} -cropleft ${values_array[2]} -cropright ${values_array[3]} $this_date/"$i"vc/image%d.jpg
		./imager $this_date/"$i"vc &
		sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
	done

	speeds=$( for i in `cat protocols/random.txt`; do echo "$RANDOM $i"; done | sort | sed -r 's/^[0-9]+ //' | awk '{print $0; if(NR % 1 == 0) print "0"}' | sed '$d' )
	for i in ${speeds}
	do
		if [ "$i" -eq "0" ]
		then
			sudo ./motor/close
			sleep 2
			sudo ./motor/11v
			sleep 2
			sudo ./motor/close
			sleep 2
			sudo ./motor/11v
			sleep 2
			sudo ./motor/close
			sleep 2
			sudo ./motor/11v
			sleep 2
			sudo ./motor/close
			continue
		fi
		sudo ./motor/"$i"v
		echo "$i"v >> $this_date/order.txt
		date | cat >> $this_date/order.txt
		ffmpeg -f video4linux2 -i /dev/video0 -t $coll_time -r .5 -s 640x480 $this_date/"$i"va_video.wmv
		ffmpeg -i $this_date/"$i"va_video.wmv -t $coll_time -r .5 -s 640x480 -croptop ${values_array[0]} -cropbottom ${values_array[1]} -cropleft ${values_array[2]} -cropright ${values_array[3]} $this_date/"$i"va/image%d.jpg
		./imager $this_date/"$i"va &
		sudo ./motor/close
		sleep 2
		sudo ./motor/11v
		sleep 2
		sudo ./motor/close
		sleep 2
		sudo ./motor/11v
		sleep 2
		sudo ./motor/close
		sleep 2
		sudo ./motor/11v
		sleep 2
		sudo ./motor/close
	done
fi

if [ $choice -eq 3 ]
then
	speeds=$( cat protocols/lg_incr.txt )
	for i in ${speeds}
	do
		#if [ "$i" -eq "0" ]
		#then
		#	sudo ./motor/close
		#	sleep 1200  
		#	continue
		#fi
		sudo ./motor/"$i"v
		echo "$i"v >> $this_date/order.txt
		date | cat >> $this_date/order.txt
		ffmpeg -f video4linux2 -i /dev/video0 -t $coll_time -r .5 -s 640x480 $this_date/"$i"vc_video.wmv
		ffmpeg -i $this_date/"$i"vc_video.wmv -t $coll_time -r .5 -s 640x480 -croptop ${values_array[0]} -cropbottom ${values_array[1]} -cropleft ${values_array[2]} -cropright ${values_array[3]} $this_date/"$i"vc/image%d.jpg
		./imager $this_date/"$i"vc &
		sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
	done

	speeds=$( for i in `cat protocols/random.txt`; do echo "$RANDOM $i"; done | sort | sed -r 's/^[0-9]+ //' | awk '{print $0; if(NR % 1 == 0) print "0"}' | sed '$d' )
	for i in ${speeds}
	do
		if [ "$i" -eq "0" ]
		then
			sudo ./motor/close
			sleep 2
			sudo ./motor/11v
			sleep 2
			sudo ./motor/close
			sleep 2
			sudo ./motor/11v
			sleep 2
			sudo ./motor/close
			sleep 2
			sudo ./motor/11v
			sleep 2
			sudo ./motor/close
			continue
		fi
		sudo ./motor/"$i"v
		echo "$i"v >> $this_date/order.txt
		date | cat >> $this_date/order.txt
		ffmpeg -f video4linux2 -i /dev/video0 -t $coll_time -r .5 -s 640x480 $this_date/"$i"va_video.wmv
		ffmpeg -i $this_date/"$i"va_video.wmv -t $coll_time -r .5 -s 640x480 -croptop ${values_array[0]} -cropbottom ${values_array[1]} -cropleft ${values_array[2]} -cropright ${values_array[3]} $this_date/"$i"va/image%d.jpg
		./imager $this_date/"$i"va &
		sudo ./motor/close
		sleep 2
		sudo ./motor/11v
		sleep 2
		sudo ./motor/close
		sleep 2
		sudo ./motor/11v
		sleep 2
		sudo ./motor/close
		sleep 2
		sudo ./motor/11v
		sleep 2
		sudo ./motor/close
	done

	speeds=$( cat protocols/sm_incr.txt )
	for i in ${speeds}
	do
		#if [ "$i" -eq "0" ]
		#then
		#	sudo ./motor/close
		#	sleep 1200  
		#	continue
		#fi
		sudo ./motor/"$i"v
		echo "$i"v >> $this_date/order.txt
		date | cat >> $this_date/order.txt
		ffmpeg -f video4linux2 -i /dev/video0 -t $coll_time -r .5 -s 640x480 $this_date/"$i"vb_video.wmv
		ffmpeg -i $this_date/"$i"vb_video.wmv -t $coll_time -r .5 -s 640x480 -croptop ${values_array[0]} -cropbottom ${values_array[1]} -cropleft ${values_array[2]} -cropright ${values_array[3]} $this_date/"$i"vb/image%d.jpg
		./imager $this_date/"$i"vb &
		sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
		#sleep 2
		#sudo ./motor/11v
		#sleep 2
		#sudo ./motor/close
	done
fi

#if [ $choice -eq 4 ]
#then
#	speeds=$( cat protocols/manual.txt )
#fi


#for i in ${speeds}
#do
#	if [ "$i" -eq "0" ]
#	then
#		sudo ./motor/close
#		sleep 1200  
#		continue
#	fi
#	sudo ./motor/"$i"v
#	ffmpeg -f video4linux2 -i /dev/video0 -t $coll_time -r 1 -s 640x480 $this_date/"$i"v_video.wmv
#	ffmpeg -i $this_date/"$i"v_video.wmv -t $coll_time -r 1 -s 640x480 -croptop ${values_array[0]} -cropbottom ${values_array[1]} -cropleft ${values_array[2]} -cropright ${values_array[3]} $this_date/"$i"v/image%d.jpg
#	./imager $this_date/"$i"v &
#done

sudo ./motor/close
