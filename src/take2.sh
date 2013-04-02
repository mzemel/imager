#! /bin/bash

date_var=$( date '+%Y_%m_%d')
mkdir $date_var
choice=$(((date '+%S')%6))
if $choice -eq 0
then
	choice_order=(3 1 2)
elif $choice -eq 1
then
	choice_order=(3 2 1)
elif $choice -eq 2
then
	choice_order=(2 1 3)
elif $choice -eq 3
then
	choice_order=(2 3 1)
elif $choice -eq 4
then
	choice_order=(1 2 3)
elif $choice -eq 5
then
	choice_order=(1 3 2)
elif
fi
#At this point I've made a folder based on the date and generated a random number (held by $choice) from 0-5, then I assigned $choice_order to hold a random combo.

if ${choice_order[0]} -eq 1
then
	
