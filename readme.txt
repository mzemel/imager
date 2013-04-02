This file was written by Michael Zemel, do what you will with it.

Imager - The Reason I Have No Friends

1)  You need CImg image library installed and included in your /usr/include directory for this to work.  They have better instructions than me on how to do this.

2)  Bugs:

Cropping is appearing to be an issue - if you don't crop a photo, it's coming up as "unable to recognize format".  Must look into this.

5/3/12:

Current state of affairs:

We had been using the uEye camera to capture images; this provided ultimately to be a fruitless task - images were both of poor quality and large size.  It's a shame that the amount of work put into understanding a relatively complex device did not yield results.  Anyway, it was found that the IPEVO Point 2 View camera originally used late 2011 on Windows XP, while having no Linux drivers, can be used by virtually any capture program using V4L2.  Video playback is a bear, as the videos are dark red (seems to be a common, more global problem on this Linux machine) BUT images come out fine (as I suspect videos do, too.)

So:  todd.sh is a script to control the motor and streamer, used to capture images.  It is not yet linked with imager, but that will come soon.  This new system renders a couple things useless: uEye (and anywhere you see it), "crop" (crop.cpp), run.sh.  Whole lotta stuff dumped into misc.  Obviously crop will be re-integrated, most likely using batch processing a la ffmpeg to crop the photos we've taken.  Perhaps this will be at the front of the event chain, with the user being shown a still from the camera and asked where to crop for our streamer captures.  This sounds wise.  Lastly, the folder "motor" contains all the executables to control the motor.

To do:
-Set crop for before streamer captures
-Link up imager just with the images that will be dumped into their respective folders (possibly running in the background as the next speed trial takes place)
-Maximize number of images that can be processed within one hour by imager
-Computer control to turn off motor afterwards
-Upgrade to Ubuntu 12.04

As for protocol, simply type "sudo ./~/Desktop/imager/todd.sh" and enter the password (fish) and the behavior experiments should run through with the chips we currently have.


5/20/12:

Email from today:

"Hi,

So the analysis program should run.  Check out the before and after gifs, as well as a time lapse.  This is using a couple pipette tips floating downstream over 20 seconds.  You can see it's discriminating enough to count the black only; not the blue segment.  I'm hoping that it'll work well with a small fish - I'm almost positive it will with a swimming one.  Anyway, if _anyone_ has feedback or a way I can refine this, let me know.

I guess we have a lot of dead fish.  No idea why that happened.

See you all tomorrow morning!

PS the time lapse is inverted, with the lightest stuff showing the most activity/darkest stuff.  Or something.
Hi,

So the analysis program should run.  Check out the before and after gifs, as well as a time lapse.  This is using a couple pipette tips floating downstream over 20 seconds.  You can see it's discriminating enough to count the black only; not the blue segment.  I'm hoping that it'll work well with a small fish - I'm almost positive it will with a swimming one.  Anyway, if _anyone_ has feedback or a way I can refine this, let me know.

I guess we have a lot of dead fish.  No idea why that happened.

See you all tomorrow morning!

PS the time lapse is inverted, with the lightest stuff showing the most activity/darkest stuff.  Or something."

Email from 5/16/12:

"Hey,

It's late and I'm heading back now, so I just wanted to give you an update on progress that's been made today:

I wrote a new program for cropping and batch processing the image files.  The old one was built around cropping a video into images.  Granted, that part of it worked really well, but now that we're using straight images I wrote something to best fit it.  Attached is a .gif of it in action; pretty cool.

So it'll crop all of our images really quick, and I incorporated what we'd talked about with just using the color of the fish to eliminate all the background.  Compare "New_Picture.jpg" with "Old_Picture.jpg" for an idea of how the new, simple subtraction method compares with the old.  I'd thought it was sufficient, but the old method is just tantalizing.  That said, it does require the fish to swim around in order to really get that contrast, and I haven't found a good way to tweak it.

The math isn't complicated, so if David, Nicholas or Jason is interested in sitting down with me to come up with a better algorithm, I'd really appreciate that.

PS:

Some last notes: obviously, the old picture involves a larger fish and the image recording distance for the new protocol leaves something to be desired.  I imagine this can be taken care of either by enlarging the picture (sure, losing a little resolution) or altering the snapshot program that we have running.

Anyway, that's all.  Right now, it's pretty streamlined.  If you have a folder in the program directory called "05/12/2012" or whatever we've been saving them as, you just have to type ./shell.sh 05/12/2012 and it'll go through it recursively and crop all the images.  That's where I'm at.

Source code attached for anyone who wants to look through it!  (well-documented so far)"

5/22:

Changed imager.cpp to read image files that were %d.jpg rather than image%d.jpg.  CImg seems to have a problem with [CImg object].load("really/damn/long/string/image%d.jpg") so I'm cutting down as much as I can from it.

Arg what the fuck.  Tried to analyze 700 sequential pictures from old_data/3_21/8 (good data).  Kept getting trouble reading the format (see above) so I only used 300 pictures.  Couldn't crop them with ffmpeg for whatever reason - no idea - and then got a segmentation fault from imager.  ARGHHH

But it looks like the real program works really well when on the 120s test bit.  Hell yeah!

5/24:

To do:

create text files in imager directory that todd.sh will read from

this way there can be one for protocol a, protocol b, protocol c, plus the user can make one

when todd boots up, it'll ask which file to read from.

Also, either open up camera earlier or stop the first two frames?  Or does it not matter with a long enough collection time?

7/30:

Work has begun on dumping the image info into a database.  Unfortunately, I've run into a problem with housing more than ~1800 images in memory.  It works fine otherwise (minor fixups needed include better formatting of output data, handling of unreadable images, and better error handling all around) but it works.  Anyway, experimented with MySQL++ but couldn't get it to work, now using SQLite3, which seems to work.

Actually, it seems that a text file can't handle an SQL command creating a table with 300,000 columns (one for each pixel).  I'll have to look into blob storage.

Or maybe create a table for each pixel with room for ROW 1: IMAGE NUMBER && ROW 2: INTENSITY

8/5:

I've finished a Calc Basic macro to take data, sumif it, sort it by angle, and filter out the duplicates.  This needs to be preceded by a rounding function, but can be applied to any sheet of data.  Yay.
