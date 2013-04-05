//Notes: 5/20 I switched "bottom_crop" and "right_crop" to be the [image height minus y-coordinate of mouse] and [image width minus x-coordinate of mouse] respectively.  Before, it was not congruous (congruing?) there was incongruity with ffmpeg cropping.

#include <iostream>
#include "CImg.h"
#include "math.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "time.h"
#include <X11/Xlib.h>
using namespace cimg_library;
using namespace std;

//Basically ignore this function (intensity).  It's just on it's own so I can quickly query the quoridinates of any image and get its grayscale intensity.
float intensity(int x, int y, CImg<unsigned char> image){
	return (0.2126*((float)image.atXY(x,y,0,0))+0.7152*((float)image.atXY(x,y,0,1))+0.0722*((float)image.atXY(x,y,0,2)));
}
	
//So the gist of this:
//It takes, as an argument from "shell.sh", a character array of the name of that image I mentioned in "shell.sh".  If you're just joining us now, it's just the third image (i0002.jpeg) from the first flow speed of whatever folder got yanked when "shell.sh" was invoked.  Random image, not really important which is used.
int main(int argc, char* argv[]){
	string str = argv[1];
	char snapshot[(int)str.length()];
	for (int i=0;i<(int)str.length();i++){
		snapshot[i]=str[i];
	}
	snapshot[(int)str.length()]='\0';
	CImg<unsigned char> image(snapshot);
	CImg<unsigned char> derp = image;
//Here we're declaring 'image' (poorly named) to be a CImg object based on i0002.jpeg.  It has lovely properties that will be used throughout the analysis programs; all the CImg library classes, methods, whatever, can be found here: http://cimg.sourceforge.net/reference/group__cimg__tutorial.html

//Also, I'm creating a duplicate CImg object, 'derp', to fool around with (such as having an orange line track your mouse when you hover over the image later.  This way we can mess with 'derp' but leave the purity of 'image' intact.
	CImg<unsigned char> visu(image.width(),image.height(),1,3,0);
	visu.fill(255);
	CImgDisplay main_disp(derp);
	//float fish_color;
	const unsigned char color[] = { 255,128,64 };
	int top_crop = 0, left_crop = 0, right_crop = 0, bottom_crop = 0;
	int count = 0;
//Declaring what are going to be my crop values: how many pixels from the left, right, etc.
//int count is a convoluted solution to not having a way of saying "Ok, this first click does this, this second click does this."  This way, when you click, it ups the count of 'count' and takes you to the proper case below.
	while (!main_disp.is_closed() && count<4){
		main_disp.display(derp);
		main_disp.wait();

//Cropping: The CImg object "image" is loaded from the image file (test.jpeg) and a duplicate CImg object called "derp" is created for temporary manipulation.  It will display "derp" with orange lines for cropping as the user moves the mouse.  Only when they click does it actually crop anything, doing so from the root "image" object.  This means the original copy is altered and any changes can only be reset by reloading it from the directory.  From this, we'll get four values, representing how close in on each side to crop out the extra stuff, and also a value for the color or intensity of the fish after it's all cropped.

//Crop top:
		if (main_disp.mouse_x()>0 && main_disp.mouse_x()<image.width() && main_disp.mouse_y()>0 && main_disp.mouse_y()<image.height() && count==0){
			main_disp.set_title("Crop top");
			derp = image;
			derp.draw_line(0,main_disp.mouse_y(),(image.width())/11,main_disp.mouse_y(),color);
			derp.draw_line((2*(image.width())/11),main_disp.mouse_y(),(3*(image.width())/11),main_disp.mouse_y(),color);
			derp.draw_line((4*(image.width())/11),main_disp.mouse_y(),(5*(image.width())/11),main_disp.mouse_y(),color);
			derp.draw_line((6*(image.width())/11),main_disp.mouse_y(),(7*(image.width())/11),main_disp.mouse_y(),color);
			derp.draw_line((8*(image.width())/11),main_disp.mouse_y(),(9*(image.width())/11),main_disp.mouse_y(),color);
			derp.draw_line((10*(image.width())/11),main_disp.mouse_y(),(11*(image.width())/11),main_disp.mouse_y(),color);
			if (main_disp.button()){
				top_crop = main_disp.mouse_y();
				for (int i=0;i<image.width();i++){
					for (int j=0;j<top_crop;j++){
						image(i,j,0,0) = 255;
						image(i,j,0,1) = 255;
						image(i,j,0,2) = 255;
					}
				}
				count++;
			}
		}
//Crop bottom:
		else if (main_disp.mouse_x()>0 && main_disp.mouse_x()<image.width() && main_disp.mouse_y()>0 && main_disp.mouse_y()<image.height() && count==1){
			main_disp.set_title("Crop bottom");
			derp = image;
			derp.draw_line(0,main_disp.mouse_y(),(image.width())/11,main_disp.mouse_y(),color);
			derp.draw_line((2*(image.width())/11),main_disp.mouse_y(),(3*(image.width())/11),main_disp.mouse_y(),color);
			derp.draw_line((4*(image.width())/11),main_disp.mouse_y(),(5*(image.width())/11),main_disp.mouse_y(),color);
			derp.draw_line((6*(image.width())/11),main_disp.mouse_y(),(7*(image.width())/11),main_disp.mouse_y(),color);
			derp.draw_line((8*(image.width())/11),main_disp.mouse_y(),(9*(image.width())/11),main_disp.mouse_y(),color);
			derp.draw_line((10*(image.width())/11),main_disp.mouse_y(),(11*(image.width())/11),main_disp.mouse_y(),color);
			if (main_disp.button()){
				bottom_crop = main_disp.mouse_y();
				for (int i=0;i<image.width();i++){
					for (int j=bottom_crop;j<image.height();j++){
						image(i,j,0,0) = 255;
						image(i,j,0,1) = 255;
						image(i,j,0,2) = 255;
					}
				}
				count++;
			}
		}
//Crop left:
		else if (main_disp.mouse_x()>0 && main_disp.mouse_x()<image.width() && main_disp.mouse_y()>0 && main_disp.mouse_y()<image.height() && count==2){
			main_disp.set_title("Crop left");
			derp = image;
			derp.draw_line(main_disp.mouse_x(),(0*(image.height())/11),main_disp.mouse_x(),(1*(image.height())/11),color);
			derp.draw_line(main_disp.mouse_x(),(2*(image.height())/11),main_disp.mouse_x(),(3*(image.height())/11),color);
			derp.draw_line(main_disp.mouse_x(),(4*(image.height())/11),main_disp.mouse_x(),(5*(image.height())/11),color);
			derp.draw_line(main_disp.mouse_x(),(6*(image.height())/11),main_disp.mouse_x(),(7*(image.height())/11),color);
			derp.draw_line(main_disp.mouse_x(),(8*(image.height())/11),main_disp.mouse_x(),(9*(image.height())/11),color);
			derp.draw_line(main_disp.mouse_x(),(10*(image.height())/11),main_disp.mouse_x(),(11*(image.height())/11),color);
			if (main_disp.button()){
				left_crop = main_disp.mouse_x();
				for (int i=0;i<left_crop;i++){
					for (int j=0;j<image.height();j++){
						image(i,j,0,0) = 255;
						image(i,j,0,1) = 255;
						image(i,j,0,2) = 255;
					}
				}
				count++;
			}
		}
//Crop right:
		else if (main_disp.mouse_x()>0 && main_disp.mouse_x()<image.width() && main_disp.mouse_y()>0 && main_disp.mouse_y()<image.height() && count==3){
			main_disp.set_title("Crop right");
			derp = image;
			derp.draw_line(main_disp.mouse_x(),(0*(image.height())/11),main_disp.mouse_x(),(1*(image.height())/11),color);
			derp.draw_line(main_disp.mouse_x(),(2*(image.height())/11),main_disp.mouse_x(),(3*(image.height())/11),color);
			derp.draw_line(main_disp.mouse_x(),(4*(image.height())/11),main_disp.mouse_x(),(5*(image.height())/11),color);
			derp.draw_line(main_disp.mouse_x(),(6*(image.height())/11),main_disp.mouse_x(),(7*(image.height())/11),color);
			derp.draw_line(main_disp.mouse_x(),(8*(image.height())/11),main_disp.mouse_x(),(9*(image.height())/11),color);
			derp.draw_line(main_disp.mouse_x(),(10*(image.height())/11),main_disp.mouse_x(),(11*(image.height())/11),color);
			if (main_disp.button()){
				right_crop = main_disp.mouse_x();
				for (int i=right_crop;i<image.width();i++){
					for (int j=0;j<image.height();j++){
						image(i,j,0,0) = 255;
						image(i,j,0,1) = 255;
						image(i,j,0,2) = 255;
					}
				}
				count++;
			}
		}

//OK, now we have the crop values (top_crop, etc.)  We're gonna hold on to them and dump them in the text file "crop.txt" at the end of this.


//This below lets the user click on the fish and get an intensity value for it.  Then we'll eliminate everything that's not within 30 units.  For reference, the fish is usually intensity of ~18 (closer to 0 meaning black) and the background is ~155 (out of a possible 255).  So 30 seems to be an alright margin for including all the colors of the fish.  However, it's hard to completely crop out the sides, and we end up getting tiny little artifacts spread throughout the resolved photo.
//This will probably be fixed when we try to calculate the fish's orientation by looking for clusters - it seems so far that really only the fish shows a real cluster; the extra artifacts do remain quite sparse.
		/*else if (count==4){
			main_disp.set_title("Click on the fish");
			derp = image;
			if (main_disp.button()){
				fish_color = intensity(main_disp.mouse_x(),main_disp.mouse_y(),image);
				cout << main_disp.mouse_x() << ", " << main_disp.mouse_y() << endl;
				cout << fish_color << endl;
				cout << image.width() << ", " << image.height() << endl;
				for (int i=0;i<image.width();i++){
					for (int j=0;j<image.height();j++){
						if (abs(intensity(i,j,image)-fish_color)<30){
							visu(i,j,0,0) = (int)intensity(i,j,image);
							visu(i,j,0,1) = (int)intensity(i,j,image);
							visu(i,j,0,2) = (int)intensity(i,j,image);
						}
					}
				}
				count++;
			}
		}*/
//Right now the above is blocked out because the program imager is going to be handling 'subtracting what's not a fish' from the pictures.  Albeit the algorithm in imager needs to be refined, for right now it's still handling that.
//If you want to add this section back into the program, change the "&& count<4" in the 'while' conditional to "&& count<5"
//Also, I took out the defintion of "fish_color", as well as the part later that puts it in "crop.txt", as well as the sprintf command preceding that.

	}
//Yada yada, just dumping all the crop values into the text file.  For good measure, I'm throwing in the intensity value of the fish, too.
	int proper_bottom_crop = (image.height()-bottom_crop);
	int proper_right_crop = (image.width()-right_crop);
	int cleft = 0, cright = 0, ctop = 0, cbottom = 0;
	cleft = left_crop;
	cright = proper_right_crop;
	ctop = top_crop;
	cbottom = proper_bottom_crop;
	//char cfish_color[50];
	//sprintf(ctop,"%d", top_crop);
	//sprintf(cbottom,"%d", proper_bottom_crop);
	//sprintf(cleft,"%d",left_crop);
	//sprintf(cright,"%d", proper_right_crop);
	//sprintf(cfish_color,"%f",fish_color);
	
	string dir_beta = str;
	dir_beta.erase(11);

	//FILE * pFile;
	dir_beta.append("crop.txt");
	//cout << dir_beta << endl;
	//char cropfile[(int)dir_beta.length()];
	//for (int i=0;i<(int)dir_beta.length();i++){
	//	cropfile[i]=dir_beta[i];
	//}
	//cropfile[(int)str.length()]='\0';

	/*pFile = fopen(cropfile, "a");
	fputs (ctop,pFile);
	fputs ("\n",pFile);
	fputs (cbottom,pFile);
	fputs ("\n",pFile);
	fputs (cleft,pFile);
	fputs ("\n",pFile);
	fputs (cright,pFile);
	fputs ("\n",pFile);
	//fputs (cfish_color,pFile);
	//fputs ("\n",pFile);
	fclose (pFile);*/


	//as per: https://bbs.archlinux.org/viewtopic.php?pid=1147323#p1147323
	int in_w = 640-cleft-cright;
	if (in_w <= 0){in_w = 640;}
	string in_w_str = to_string(in_w);
	//sprintf(in_w_str,"%d", in_w);


	int in_h = 480-(int)ctop-(int)cbottom;
	if (in_h <= 0){in_h = 480;}
	string in_h_str = to_string(in_h);
	//sprintf(in_h_str,"%d", in_h);


	string x = to_string(cleft);
	string y = to_string(ctop);
	//sprintf(x,"%d", cleft);
	//sprintf(y,"%d", ctop);


	string cropstring;
	cropstring.append(in_w_str);
	cropstring.append(":");
	cropstring.append(in_h_str);
	cropstring.append(":");
	cropstring.append(x);
	cropstring.append(":");
	cropstring.append(y);

	ofstream CropFile;
	CropFile.open(dir_beta.c_str(),std::ios::app);
	CropFile << cropstring << "\n";
	CropFile.close();

	//CImgDisplay visu_disp(visu);
	//while (!visu_disp.is_closed()){
	//	visu_disp.set_title("Looks good, chief!");
	//	visu_disp.wait();//wait some # secs
	//}

	cout << "Top crop: " << top_crop << endl;
	cout << "Bottom crop: " << proper_bottom_crop << endl;
	cout << "Left crop: " << left_crop << endl;
	cout << "Right crop: " << proper_right_crop << endl;

	return 0;
}

//And that's it!  So far...
