//Notes: 5/20 commented out call to "rectify_invalids" on line 902.  Running the program was giving me a bad time.

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
using namespace cimg_library;
using namespace std;

#define PI 3.14159265

//to do: increase cut-off for what's a valid line length
//gui for rectifying invalids
//re-do directory access to be able to access dirs from arguments passed


//log.txt is moved over for >1 arg
//same with histogram.png and histogram.txt
//output images are taken care of

//getting that log.txt in there seems to be the straw that breaks the camels back.  consolidate everything...



//Function declarations (in order of implementation):
//---------------------------------------------------
float get_std_dev(int a, int b, CImg<unsigned char> *pimages, int files_size, float *paverage_by_pixel, int x){
	float intensity, red, green, blue;
	float sum=0, average, sum2=0;
	float math3;
	float deviation[files_size-2];
	for (int j=0;j<(files_size-2);j++){
		red = (float)pimages[j].atXY(a,b,0,0);
		green = (float)pimages[j].atXY(a,b,0,1);
		blue = (float)pimages[j].atXY(a,b,0,2);
		intensity = ((0.2126*(red))+(0.7152*(green))+(0.0722*(blue)));
		sum+=intensity;
	}
	average = sum/(files_size-2);
	paverage_by_pixel[a+b*x] = average;
	for (int k=0;k<(files_size-2);k++){
		red = (float)pimages[k].atXY(a,b,0,0);
		green = (float)pimages[k].atXY(a,b,0,1);
		blue = (float)pimages[k].atXY(a,b,0,2);
		intensity = ((0.2126*(red))+(0.7152*(green))+(0.0722*(blue)));
		deviation[k]=abs(intensity-average);
		deviation[k]*=deviation[k];
		sum2+=deviation[k];
	}
	math3 = (float)(sum2/(files_size-3));
	math3 = sqrt(math3);
	return math3;
}
//Used for each pixel.  Finds the standard deviation through all images for that pixel and returns it as a float.

CImg<unsigned char> normalized_std_dev_display(float* pstd_dev, float* pnormalized_std_dev, int x, int y){
	float math1=0;
	for (int i=0;i<x*y;i++){//sets normalized_std_dev[] for display
		math1 += pstd_dev[i];
	}
	math1 = math1/(x*y);
	for (int i=0;i<x*y;i++){
		pnormalized_std_dev[i]=pstd_dev[i]*(127/math1);
	}

	CImg<unsigned char> disp (x,y);//display normalized_std_dev
	disp.fill(0);
	for (int b=0;b<y;b++){
		for (int a=0;a<x;a++){
			if (pnormalized_std_dev[a+b*x]>255){
				pnormalized_std_dev[a+b*x]=255;
			}
//			cout << pnormalized_std_dev[a+b*x] << ", ";
			disp(a,b)=(int)pnormalized_std_dev[a+b*x];
		}
	}
	return disp;
}
//Assigns characteristics to a CImg object representing a normalized time-lapse of all images, with activity relatively lightened.

void catchdeviants(float *pstd_dev, int *pdeviantcount, CImg<unsigned char>* pimages, int x, int y, int files_size){
	float intensity1, intensity2, red1, blue1, green1, red2, green2, blue2;
	pdeviantcount[0]=0;
	for (int i=0;i<(files_size-3);i++){
		pdeviantcount[i+1]=0;
		for (int j=10;j<(y-10);j++){//I could pad the scan here to not include the frame.
			for (int k=10;k<(x-10);k++){//I'm doin' it.
				red1 = (float)pimages[i].atXY(k,j,0,0);
				green1 = (float)pimages[i].atXY(k,j,0,1);
				blue1 = (float)pimages[i].atXY(k,j,0,2);
				intensity1 = ((0.2126*(red1))+(0.7152*(green1))+(0.0722*(blue1)));
				red2 = (float)pimages[i+1].atXY(k,j,0,0);
				green2 = (float)pimages[i+1].atXY(k,j,0,1);
				blue2 = (float)pimages[i+1].atXY(k,j,0,2);
				intensity2 = ((0.2126*(red2))+(0.7152*(green2))+(0.0722*(blue2)));
				if (abs(intensity2-intensity1)>4*pstd_dev[k+j*x] && intensity2<100){
//				if pixel is more than one standard deviation different
					pdeviantcount[i+1]+=1;
				}
				else continue;
			}
		}
	}
}
//Assigns value to the array deviantcount[], which is the number of deviant pixels (those greater than something like 2.5x the standard deviation for that pixel.)  The implication being an image needs to reach a certain # of "deviant pixels" in order to be considered unique from the one preceding it.

CImg<unsigned char> orientation(CImg<unsigned char> image, float &angle, int &validity){
	int x = image.width();
	int y = image.height();
	int left_bound=x,right_bound=0,top_bound=y,bottom_bound=0;
	int offset=0;
	for (int i=offset;i<y-offset;i++){
		for (int j=offset;j<x-offset;j++){
			if ((int)image.atXY(j,i)!=255 && (((int)image.atXY((j-3),i)!=255 && (int)image.atXY((j+3),i)!=255) && ((int)image.atXY(j,(i-3))!=255 && (int)image.atXY(j,(i+3))!=255))){
				if (i<top_bound) top_bound=i;
				else if (i>bottom_bound) bottom_bound=i;
				else if (j<left_bound) left_bound=j;
				else if (j>right_bound) right_bound=j;
			}
			else continue;
		}
	}
	if ((left_bound==x && right_bound==0) | (top_bound==y && bottom_bound==0)) return image; // IMPORTANT - change to "continue" in a test2.cpp
	if (left_bound>(3+offset)) left_bound-=3;							// change to "return image" or "temp" in test1.cpp
	if (right_bound<(x-(3+offset))) right_bound+=3;
	if (top_bound>(3+offset)) top_bound-=3;
	if (bottom_bound<(y-(3+offset))) bottom_bound+=3;
	cout << right_bound-left_bound << ", ";
	cout << bottom_bound-top_bound << ", ";
	int white_count,black_count;
	int top_black_count[50]={0};
	float angle_contender[50]={0};
	int a_c_x[50]={0}, a_c_y[50]={0};
	int top_top_black_count=0;
	for (int i=top_bound;i<bottom_bound;i++){
		for (float angle=0;angle<(PI/2);angle+=(PI/180)){
			white_count=0,black_count=0;
			for (int r=0;r<(sqrt((right_bound-left_bound)*(right_bound-left_bound)+(bottom_bound-top_bound)*(bottom_bound-top_bound)));r++){
				int rcos = (int)(r*cos(angle));
				int rsin = (int)(r*sin(angle));
				if (((left_bound+rcos)>right_bound) | ((i-rsin)<=top_bound) | (white_count>9)){
					for (int g=0;g<50;g++){
						if (black_count>top_black_count[g]){
							if (black_count>top_top_black_count) top_top_black_count=black_count;
							top_black_count[g]=black_count;
							angle_contender[g]=angle;
							a_c_x[g]=left_bound;
							a_c_y[g]=i;
							break;
						}
						else continue;
					}
					break;
				}
				else {
					if ((int)image.atXY((left_bound+rcos),(i-rsin))==255 && black_count==0) continue;
					else if ((int)image.atXY((left_bound+rcos),(i-rsin))!=255) black_count++;
					else if ((int)image.atXY((left_bound+rcos),(i-rsin))==255 && black_count!=0) white_count++;
					else {
						return image;
					}
				}
			}
			//cout << "Angle good: " << angle << endl;
		}
	}
	for (int i=left_bound;i<right_bound;i++){
		for (float angle=0;angle<(PI/2);angle+=(PI/180)){
			white_count=0,black_count=0;
			for (int r=0;r<(sqrt((right_bound-left_bound)*(right_bound-left_bound)+(bottom_bound-top_bound)*(bottom_bound-top_bound)));r++){
				int rcos = (int)(r*cos(angle));
				int rsin = (int)(r*sin(angle));
				if (((i+rcos)>right_bound) | ((bottom_bound-rsin)<=top_bound) | (white_count>9)){
					for (int g=0;g<50;g++){
						if (black_count>top_black_count[g]){
							if (black_count>top_top_black_count) top_top_black_count=black_count;
							top_black_count[g]=black_count;
							angle_contender[g]=angle;
							a_c_x[g]=i;
							a_c_y[g]=bottom_bound;
							break;
						}
						else continue;
					}
					break;
				}
				else {
					if ((int)image.atXY((i+rcos),(bottom_bound-rsin))==255 && black_count==0) continue;
					else if ((int)image.atXY((i+rcos),(bottom_bound-rsin))!=255) black_count++;
					else if ((int)image.atXY((i+rcos),(bottom_bound-rsin))==255 && black_count!=0) white_count++;
					else {
						return image;
					}
				}
			}
			//cout << "Angle good: " << angle << endl;
		}
	}
	for (int i=left_bound;i<right_bound;i++){
		for (float angle=(PI/2);angle<PI;angle+=(PI/180)){
			white_count=0,black_count=0;
			for (int r=0;r<(sqrt((right_bound-left_bound)*(right_bound-left_bound)+(bottom_bound-top_bound)*(bottom_bound-top_bound)));r++){
				int rcos = (int)(r*cos(angle));
				int rsin = (int)(r*sin(angle));
				if (((i+rcos)<left_bound) | ((bottom_bound-rsin)<=top_bound) | (white_count>9)){
					for (int g=0;g<50;g++){
						if (black_count>top_black_count[g]){
							if (black_count>top_top_black_count) top_top_black_count=black_count;
							top_black_count[g]=black_count;
							angle_contender[g]=angle;
							a_c_x[g]=i;
							a_c_y[g]=bottom_bound;
							break;
						}
						else continue;
					}
					break;
				}
				else {
					if ((int)image.atXY((i+rcos),(bottom_bound-rsin))==255 && black_count==0) continue;
					else if ((int)image.atXY((i+rcos),(bottom_bound-rsin))!=255) black_count++;
					else if ((int)image.atXY((i+rcos),(bottom_bound-rsin))==255 && black_count!=0) white_count++;
					else {
						return image;
					}
				}
			}
			//cout << "Angle good: " << angle << endl;
		}
	}
	for (int i=top_bound;i<bottom_bound;i++){
		for (float angle=(PI/2);angle<PI;angle+=(PI/180)){
			white_count=0,black_count=0;
			for (int r=0;r<(sqrt((right_bound-left_bound)*(right_bound-left_bound)+(bottom_bound-top_bound)*(bottom_bound-top_bound)));r++){
				int rcos = (int)(r*cos(angle));
				int rsin = (int)(r*sin(angle));
				if (((right_bound+rcos)<left_bound) | ((i-rsin)<=top_bound) | (white_count>9)){
					for (int g=0;g<50;g++){
						if (black_count>top_black_count[g]){
							if (black_count>top_top_black_count) top_top_black_count=black_count;
							top_black_count[g]=black_count;
							angle_contender[g]=angle;
							a_c_x[g]=right_bound;
							a_c_y[g]=i;
							break;
						}
						else continue;
					}
					break;
				}
				else {
					if ((int)image.atXY((right_bound+rcos),(i-rsin))==255 && black_count==0) continue;
					else if ((int)image.atXY((right_bound+rcos),(i-rsin))!=255) black_count++;
					else if ((int)image.atXY((right_bound+rcos),(i-rsin))==255 && black_count!=0) white_count++;
					else {
						return image;
					}
				}
			}
			//cout << "Angle good: " << angle*(180/PI) << endl;
		}
	}
	for (int d=0;d<50;d++){
		//cout << "Angle " << d << ": " << angle_contender[d]*(180/PI) << ", " << top_black_count[d] << endl;
		if (top_black_count[d]<(top_top_black_count*.8)){
			angle_contender[d]=999;
			//cout << "reset\n";
		}
	}
	float avg_ac=0;
	int avg_acx=0, avg_acy=0, avg_tbc=0;
	int catchlow=0, catchhigh=0;
	int n=0;
	for (int f=0;f<50;f++){
		if (angle_contender[f]!=999){
			if ((angle_contender[f]*(180/PI))<20) catchlow++;
			else if ((angle_contender[f]*(180/PI))>160) catchhigh++;
			else continue;
		}
		else continue;
	}
	//cout << "high: " << catchhigh << ", low: " << catchlow << endl;
	for (int f=0;f<50;f++){
		if (angle_contender[f]!=999){
			if (catchlow>catchhigh && catchhigh>5){
				if ((angle_contender[f]*(180/PI))<150){
					avg_ac+=angle_contender[f];
					avg_acx+=a_c_x[f];
					avg_acy+=a_c_y[f];
					avg_tbc+=top_black_count[f];
					n++;
				}
			}
			else if (catchhigh>catchlow && catchlow>5){
				if ((angle_contender[f]*(180/PI))>30){
					avg_ac+=angle_contender[f];
					avg_acx+=a_c_x[f];
					avg_acy+=a_c_y[f];
					avg_tbc+=top_black_count[f];
					n++;
				}
			}
			else{
			avg_ac+=angle_contender[f];
			avg_acx+=a_c_x[f];
			avg_acy+=a_c_y[f];
			avg_tbc+=top_black_count[f];
			n++;
			}
		}
		else continue;
	}
	avg_ac=avg_ac/n;
	avg_acx=avg_acx/n;
	avg_acy=avg_acy/n;
	avg_tbc=avg_tbc/n;
	CImg<unsigned char> temp = image;
	for (int l=0;l<(2*avg_tbc);l++){
		int m=avg_acx+(int)(l*cos(avg_ac));
		int n=avg_acy-(int)(l*sin(avg_ac));
		if ((int)image.atXY(m,n)==255) continue;
		else if ((int)image.atXY(m,n)!=255 && 0<=m && m<=x && 0<=n && n<=y){
			temp(m,n)=255;
		}
		else break;
	}

	int tally=0, angle_index=0;
	for (int e=0;e<50;e++){
		if (angle_contender[e]>tally && angle_contender[e]!=999){
			tally=angle_contender[e];
		}
	}
	for (int e=0;e<50;e++){
		if (angle_contender[e]==tally){
			angle_index=e;
			break;
		}
		else continue;
	}
	//cout << "Best angle: " << angle_contender[angle_index]*(180/PI) << ", x: " << a_c_x[angle_index] << ", y: " << a_c_y[angle_index] << endl;
	int xstart=0, ystart=0, xend=0, yend=0;
	int bc=0;
	int tangent, top_tangent=0, top_m, top_n;
	for (int l=0;;l++){
		/*int m=a_c_x[angle_index]+(int)(l*cos(angle_contender[angle_index]));//consider changing this to avg angle rather than longest line
		int n=a_c_y[angle_index]-(int)(l*sin(angle_contender[angle_index]));*/
		int m=avg_acx+(int)(l*cos(avg_ac));
		int n=avg_acy-(int)(l*sin(avg_ac));
		//cout << "l: " << l <<  ", m: " << m << ", n: " << n << endl;
		//cout << (int)image.atXY(m,n) << endl;
		if ((int)image.atXY(m,n)==255 && bc==0) continue;
		else if ((int)image.atXY(m,n)==255 && bc!=0){
			int probe=0;
			for (int i=0;i<10;i++){
				/*int mpi=a_c_x[angle_index]+(int)((l+i)*cos(angle_contender[angle_index]));
				int npi=a_c_y[angle_index]-(int)((l+i)*sin(angle_contender[angle_index]));*/
				int mpi=avg_acx+(int)((l+i)*cos(avg_ac));
				int npi=avg_acy-(int)((l+i)*sin(avg_ac));
				if ((int)image.atXY(mpi,npi)==255){
					probe++;
					//cout << "Probe++ " << probe << endl;
					continue;
				}
				else continue;
			}
			if (probe>=10){
				//cout << "probed!" << endl;
				xend=m, yend=n;
				break;
			}
			else continue;
		}
		else if ((int)image.atXY(m,n)!=255 && bc==0){
			xstart=m,ystart=n;
			bc++;
		}
		else if ((m<left_bound) | (m>right_bound) | (n<top_bound) | (n>bottom_bound)){
			xend=m; yend=n;
			break;
		}
		else if ((int)image.atXY(m,n)!=255 && bc!=0){//get cross-bar info
			tangent=0;
			for (int o=0;;o++){
				int p=m+o*cos(abs(PI/2-avg_ac));
				int q=n-o*sin(abs(PI/2-avg_ac));
				if ((int)image.atXY(p,q)!=255 && !((p<left_bound) | (p>right_bound) | (q<top_bound) | (q>bottom_bound))){
					tangent++;
				}
				else break;
			}
			for (int o=0;;o++){
				int p=m-o*cos(abs(PI/2-avg_ac));
				int q=n+o*sin(abs(PI/2-avg_ac));
				if ((int)image.atXY(p,q)!=255 && !((p<left_bound) | (p>right_bound) | (q<top_bound) | (q>bottom_bound))){
					tangent++;
				}
				else break;
			}
			if (tangent>top_tangent){
				top_tangent=tangent;
				top_m=m;
				top_n=n;
			}
		}
		else continue;
	}
	for (int abc=0;abc<10;abc++){//draw cross-bar
		temp.atXY(top_m+abc*cos(abs(PI-avg_ac)),top_n-abc*sin(abs(PI/2-avg_ac)))=255;
	}
	for (int cba=0;cba<10;cba++){
		temp.atXY(top_m-cba*cos(abs(PI-avg_ac)),top_n+cba*sin(abs(PI/2-avg_ac)))=255;
	}




	int startcount=0, endcount=0;
	int sq_length=20;
	for (int j=0;j<2*sq_length;j++){
		for (int i=0;i<2*sq_length;i++){
			if (((int)image.atXY((xstart-sq_length+i),(ystart-sq_length+j))!=255) && (xstart-sq_length+i)>0 && (xstart-sq_length+i)<x && (ystart-sq_length+j)>0 && (ystart-sq_length+j)<y) startcount++;
		}
	}
	for (int j=0;j<2*sq_length;j++){
		for (int i=0;i<2*sq_length;i++){
			if (((int)image.atXY((xend-sq_length+i),(yend-sq_length+j))!=255) && (xstart-sq_length+i)>0 && (xstart-sq_length+i)<x && (ystart-sq_length+j)>0 && (ystart-sq_length+j)<y) endcount++;
		}
	}

	if (startcount>endcount && sqrt((xstart-top_m)*(xstart-top_m) + (ystart-top_n)*(ystart-top_n))>sqrt((xend-top_m)*(xend-top_m) + (yend-top_n)*(yend-top_n))){//use cross-bar proximity as a metric of whether or not it should be flipped
		avg_ac+=PI;
	}


	//cout << "Xstart: " << xstart << ", Ystart: " << ystart << ", startcount: " << startcount << endl;
	//cout << "Xend: " << xend << ", Yend: " << yend << ", endcount: " << endcount << endl;

	/*for (int f=0;f<50;f++){
		if (angle_contender[f]!=999 && avg_tbc>=50){
			cout << "Angle " << f << ": " << angle_contender[f]*(180/PI) << " degrees at (" << a_c_x[f] << ", " << a_c_y[f] << "), " << "r = " << top_black_count[f] << endl;
		}
		else if (avg_tbc<50){
			cout << "Results not valid!\n";
			break;
		}
		else continue;
	}*/
	//cout << "Norm'd Avg Angle: " << avg_ac*(180/PI) << ", X: " << avg_acx << ", Y: " << avg_acy << ", Distance: " << avg_tbc << endl;
	cout << avg_tbc << ", " << avg_ac*(180/PI);
	if (avg_tbc<50) validity=0;
	else validity=avg_tbc;
	angle=avg_ac*(180/PI); //use with a &angle parameter
	
	if (angle>=0 && angle<90){
		angle=90-angle;
	}
	else if (angle>=90 && angle<270){
		angle-=90;
	}
	else if (angle>=270 && angle<360){
		angle=450-angle;
	}

	//temp.display();
	return temp;
}
//Takes a get_deviants-processed CImg and calculates the angle (correcting for which end is the head), how "valid" it is, and returns it as a CImg to get_deviants, where the angle, validity, and time info are logged.  "Validity" refers to how good of a fit I think the proposed angle is - this is done by drawing n lines through its long axis and comparing it with the average angle (which is good in the case of 25 0-degrees & 25 179-degrees).  Images with poor "validity" by default are not included in the data.



void get_deviants(int files_size, int *pdeviantcount, vector<string> &files, int x, int y, CImg<unsigned char> *pimages, float *pstd_dev, float *paverage_by_pixel, vector<string> &invalids, vector<int> &angles, vector<int> &times, vector<int> &invalids_time, string &outputstring, string &dir){
	int math1=0;
	int remainder=0;
	string str;
	CImg<unsigned char> temp;
	float darksum, darkavg=0, darkcount;
	float lightsum, lightavg=0, lightcount;
	float red1, green1, blue1, red2, green2, blue2, intensity1, intensity2;
	for (int i=0;i<(files_size-3);i++){
		math1+=pdeviantcount[i+1];
	}
	math1=(math1/(files_size-3));//math1 = average deviant # pixels
	cout << "Deviant image threshold (no. pixels): " << (1*math1) << ".\n";


	ofstream log;
	string logstring;
	logstring = dir + "/aog.txt";
	char logchar[(int)logstring.length()];
	for (int j=0;j<(int)logstring.length();j++){
		logchar[j]=logstring[j];
	}
	logchar[(int)logstring.length()]='\0';
	log.open(logchar);
	log << "Image,Angle,Time,Validity\n";

	for (int i=0;i<(files_size-3);i++){
		if ((pdeviantcount[i+1]>(1*math1) && pdeviantcount[i+1]>20) | (i==(files_size-4))){//This should be adjustable, based on how motive the fish is.  Make the cut-off lower for more motile fish (as there'll be a higher "average deviance", making the cut-off high will not allow any images through.)  Make the cut-off lower for less motile fish.
			cout << files[i] << " (" << pdeviantcount[i+1] << ", ";
			darksum=0, darkcount=0;
			lightsum=0, lightcount=0;
			temp = pimages[i+1];
			temp.fill(255);
			if (i!=(files_size-4)){
				for (int j=0;j<y;j++){
					for (int k=0;k<x;k++){
						red1 = (float)pimages[i].atXY(k,j,0,0);
						green1 = (float)pimages[i].atXY(k,j,0,1);
						blue1 = (float)pimages[i].atXY(k,j,0,2);
						intensity1 = ((0.2126*(red1))+(0.7152*(green1))+(0.0722*(blue1)));
						red2 = (float)pimages[i+1].atXY(k,j,0,0);
						green2 = (float)pimages[i+1].atXY(k,j,0,1);
						blue2 = (float)pimages[i+1].atXY(k,j,0,2);
						intensity2 = ((0.2126*(red2))+(0.7152*(green2))+(0.0722*(blue2)));
						if (abs(intensity2-intensity1)>pstd_dev[k+j*x] && abs(intensity1-paverage_by_pixel[k+j*x])>(3*pstd_dev[k+j*x]) && intensity1<100){//if there's enough deviation and our new frame is fresh (eliminates trails)
							/*temp(k,j,0,0)=(int)intensity2;
							temp(k,j,0,1)=(int)intensity2;
							temp(k,j,0,2)=(int)intensity2;*/
							darksum+=intensity1;
							darkcount+=1;
						}
						/*if (intensity2<100){
							temp(k,j,0,0)=(int)intensity2;
							temp(k,j,0,1)=(int)intensity2;
							temp(k,j,0,2)=(int)intensity2;
						}*/
						else {
							lightsum+=intensity1;
							lightcount+=1;
						}
					}
				}
				darkavg = darksum/darkcount;
				lightavg = lightsum/lightcount;
			}
			if ((2*darkavg<lightavg) | (i==(files_size-4))){
				for (int l=10;l<y-10;l++){
					for (int m=10;m<x-10;m++){
						red1 = (float)pimages[i].atXY(m,l,0,0);
						green1 = (float)pimages[i].atXY(m,l,0,1);
						blue1 = (float)pimages[i].atXY(m,l,0,2);
						intensity1 = ((0.2126*(red1))+(0.7152*(green1))+(0.0722*(blue1)));
						/*red2 = (float)pimages[i+1].atXY(m,l,0,0);
						green2 = (float)pimages[i+1].atXY(m,l,0,1);
						blue2 = (float)pimages[i+1].atXY(m,l,0,2);
						intensity2 = ((0.2126*(red2))+(0.7152*(green2))+(0.0722*(blue2)));*/
						if (2*(abs(intensity1-darkavg))<abs(intensity1-lightavg)/* && abs(intensity1-paverage_by_pixel[m+l*x])>(.5*pstd_dev[m+l*x])*/){
							temp(m,l,0,0)=(int)intensity1;
							temp(m,l,0,1)=(int)intensity1;
							temp(m,l,0,2)=(int)intensity1;
							//note to self: consider making the pixel value simply 0 (black)?
						}
						//In plain english: if the intensity of a pixel is closer to the color of the fish 						(dark) than it is to the background (light) AND it's different enough from the 						average (assuming the fish moves around enough to make the average for any pixel 						closer to 'light' than 'dark'), then color that bastard in.
					}
				}
				cout << "*";
			}
			else{
				cout << endl;
				continue;
			}
			cout << darkavg << ", " << lightavg << ", ";
			float angle=999;
			int validity=0;
			temp = orientation(temp, angle, validity);

			string strang = files[i];
			strang.erase(0,5);
			if (strang.at(0)=='0'){
				if (strang.at(1)=='0'){
					if (strang.at(2)=='0'){
						strang.erase(0,1);
					}
					strang.erase(0,1);
				}
				strang.erase(0,1);
			}

			int strangval = (atoi(strang.c_str()));
			int time = strangval-remainder;
			remainder = strangval;

			if (angle!=999 && validity!=0){
				cout << "*";
				log << files[i] << "," << angle << "," << time*2 << "," << validity << endl;
				angles.push_back((int)angle);
				times.push_back(time); //push back time!
			}
			else if (angle==999 && time>5){
				log << files[i] << ",error,no,object" << endl; //put in rectified version to make user-friendly
				invalids.push_back(files[i]);
				invalids_time.push_back(time);
				continue;
			}
			else if (validity==0 && time>5){
				log << files[i] << "error,inconsistent,object" << endl;
				invalids.push_back(files[i]);
				invalids_time.push_back(time);
				continue;
			}
			cout << ")\n";
			str = files[i];
			if (str.at(5) == '0'){
				if (str.at(6) == '0'){
					if (str.at(7) == '0'){
						str.erase(5,3);
						str.append(".jpg");
						str.insert(0,"/");
						str.insert(0,outputstring);
						char r[(int)str.length()];
						for (int j=0;j<(int)str.length();j++){
							r[j]=str.at(j);
						}
						r[(int)str.length()]='\0';
						temp.save_jpeg(r);
					}
					else {
						str.erase(5,2);
						str.append(".jpg");
						str.insert(0,"/");
						str.insert(0,outputstring);
						char r[(int)str.length()];
						for (int j=0;j<(int)str.length();j++){
							r[j]=str.at(j);
						}
						r[(int)str.length()]='\0';
						temp.save_jpeg(r);
					}
				}
				else {
					str.erase(5,1);
					str.append(".jpg");
					str.insert(0,"/");
					str.insert(0,outputstring);
					char r[(int)str.length()];
					for (int j=0;j<(int)str.length();j++){
						r[j]=str.at(j);
					}
					r[(int)str.length()]='\0';
					temp.save_jpeg(r);
				}
			}
			else{
				str.append(".jpg");
				str.insert(0,"/");
				str.insert(0,outputstring);
				char r[(int)str.length()];
				for (int j=0;j<(int)str.length();j++){
					r[j]=str.at(j);
				}
				r[(int)str.length()]='\0';
				temp.save_jpeg(r);;
			}
		}
		else continue;
	}
	
	log.close();
}
//The real workhorse of this program, in addition to main.  It should probably be split into several functions and it does call orientation(), which is also a pretty large function.  In order, it: decides whether a frame is 'deviant enough', decides which pixels in the frame are the fish (adds up all dark unique pixels to figure out what color the fish is, then scans the image for everything close* to that color), calls orientation() (see below), opens the log.txt and fills it out, and saves all the processed images in the "output" folder.
//* I say close because this is a place where tweaking may need to occur in order to filter out the nearly-as-dark color of the walls when the water shifts.


void rectify_invalids(vector<string> &invalids, vector<int> &angles, vector<int> &times, vector<int> &invalids_time){
	int get=999;
	for (int i=0;i<(int)invalids.size();i++){
		clock_t endwait;
		endwait = clock() + 10*CLOCKS_PER_SEC;
		while (clock() < endwait){
			cout << "\nPlease examine " << invalids[i] << ".  Then enter an angle or '0' to skip.  (" << i+1 << " of " << (int)invalids.size() << ")\nAngle: ";
			cin >> get;
			if (get != '0'){
				angles.push_back(get);
				times.push_back(invalids_time[i]);
			}
			if (get!=999) break;
		}
	}
}
//The user is given the chance to open and manually enter an angle for any images with poor "validity", in which case it joins the data.

void generate_histogram(ofstream &histogram, vector<int> &angles, vector<int> &times){
	int asize = (int)angles.size();
	int atarray[360*2]={0};
	for (int a=0;a<360;a++){
		for (int j=0;j<asize;j++){
			if (angles[j]==a){
				atarray[a]++;
				atarray[a+360]+=times[j];
			}
		}
	}
	histogram << "angle time #" << endl;
	for (int b=0;b<360;b++){
		histogram << b << " " << atarray[b+360] << " " << atarray[b] << endl;
	}
}
//Produces a text file (in the "data" folder) with angle by time and # snapshots.  Essentially the culmination of this program.
//---------------------------------------------------



//First function called by main.  Opens image directory and assigns names of all contents to "files".
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
	cout << "Files scanned.\n";
    return 0;
}
//More info: "." and ".." are included in "files", which is why I reference files_size-2 at some points.  It's important that no additional files are in the directory being parsed, as I have not added anything to deal with extra items beyond "." and ".." - having more files will result in a CImg empty instance down the line.



int main(int argc, char *argv[]){


	if (argc == 1){
		cout << "Enter './run.sh' and make sure there is at least one .wmv file in this directory and that its corresponding folder (if any) has been removed.\n";
		return 0;
	}
	//If imager is run from command line instead of run.sh or if there are no .wmv files in the directory.


	vector<string> files = vector<string>();
	string dir = argv[1];
	//dir.insert(0,"./");
	getdir(dir,files);
	//See getdir comments above

	string outputstring = dir + "/output";
	char outputchar[(int)outputstring.length()];
	for (int i=0;i<(int)outputstring.length();i++){
		outputchar[i]=outputstring[i];
	}
	outputchar[(int)outputstring.length()]='\0';
	mkdir (outputchar, 0777);
	//Makes directory within image directory called "output"

	int files_size = (int)files.size();
	string str1;
	for (int i=0;i<files_size;i++){
		int k = (int)files[i].length();
		if (k==13){
			str1 = files[i];
			str1.erase(9);
			files[i]=str1;
		}
		else if (k==12){
			str1 = files[i];
			str1.erase(8);
			str1.insert(5,"0");
			files[i]=str1;
		}
		else if (k==11){
			str1 = files[i];
			str1.erase(7);
			str1.insert(5,"00");
			files[i]=str1;
		}
		else if (k==10){
			str1 = files[i];
			str1.erase(6);
			str1.insert(5,"000");
			files[i]=str1;
		}
		else {
		files[i]="null";
		}
	}
	//Changes the names of all images from "image'n'.jpg" to "image'000n'" in order to collate them ahead.  It's necessary that all images are called "image'n'.jpg" and that none exceed "image9999.jpg".  That, however, is easy to fix.  "." and ".." are renamed "null".


	string nTemp;
	for (int iCv=1;iCv<files_size;++iCv){
		nTemp = files[iCv];
		int k;
		for (k=iCv-1;k>=0&&files[k]>nTemp;k--){
			files[k+1] = files[k];
		}
		files[k+1] = nTemp;
	}
	//Rearranges "files" numerically (from image1.jpg to image9999.jpg)

	CImg<unsigned char> * pimages = new CImg<unsigned char>[files_size-2];
	//Declaration of CImg object array.  Will contain every image in order.

	for (int i=0;i<files_size;i++){
		string str = files[i];
		if (str.at(0)=='n')continue;
		else{
			if (str.at(5) == '0'){
				if (str.at(6) == '0'){
					if (str.at(7) == '0'){
						str.erase(5,3);
						str.append(".jpg");
						str.insert(0,"/");
						str.insert(0,dir);
						char r[(int)str.length()];
						for (int j=0;j<(int)str.length();j++){
							r[j]=str.at(j);
						}
						r[(int)str.length()]='\0';
						pimages[i].load(r);
					}
					else {
						str.erase(5,2);
						str.append(".jpg");
						str.insert(0,"/");
						str.insert(0,dir);
						char r[(int)str.length()];
						for (int j=0;j<(int)str.length();j++){
							r[j]=str.at(j);
						}
						r[(int)str.length()]='\0';
						pimages[i].load(r);
					}
				}
				else {
					str.erase(5,1);
					str.append(".jpg");
					str.insert(0,"/");
					str.insert(0,dir);
					char r[(int)str.length()];
					for (int j=0;j<(int)str.length();j++){
						r[j]=str.at(j);
					}
					r[(int)str.length()]='\0';
					pimages[i].load(r);
				}
			}
			else{
				str.append(".jpg");
				str.insert(0,"/");
				str.insert(0,dir);
				char r[(int)str.length()];
				for (int j=0;j<(int)str.length();j++){
					r[j]=str.at(j);
				}
				r[(int)str.length()]='\0';
				pimages[i].load(r);
			}
		}
	}
	cout << "Images loaded.\n";
	//Changes image name from "image'000n'" to "directory_name/image'n'.jpg.  Fills out pimages.



	vector<int> angles = vector<int>();
	vector<int> times = vector<int>();
	//Will end up containing angles and time duration of all valid images.  angles[k] corresponds to times[k].
	vector<string> invalids = vector<string>();
	vector<int> invalids_time = vector<int>();
	//Will end up containing angles and time duration of all invalid images.  angles[k] corresponds to times[k].


	int x = (int)pimages[0].width();
	int y = (int)pimages[0].height();
	//Creates width and height variables for easy passing to functions.  Obviously all images must be of the same dimensions.

	//-------------------------------
	//Data Arrays
	//-------------------------------
	float * pstd_dev = new float[x*y];
	//Array of standard deviation for each pixel from (0,0) to (x,y).  Interesting note: (0,0) is top-left corner.
	float *pnormalized_std_dev = new float[x*y];
	//Normalizes the above in order to create the time-lapse image: the average standard deviation becomes 128 (grey) and higher-activity pixels are pushed towards 255.
	int *pdeviantcount = new int[files_size-2];
	//A count of the # of deviant pixels for each image.  Used to determine if an image is "significantly deviant" enough to consider the fish having moved.
	float *paverage_by_pixel = new float[x*y];
	//Used to 1) calculate std_dev for each pixel and 2) act as a threshold for considering a pixel part of the fish and not part of the background in get_deviants().  Consider getting rid of this and adding local arrays in both (1) and (2).
	//-----------------------------
	//Array Filling
	//-----------------------------
	for (int b=0;b<y;b++){
		for (int a=0;a<x;a++){
			pstd_dev[a+b*x] = get_std_dev(a,b,pimages,files_size,paverage_by_pixel,x);
		}
	}
	cout << "Pixel standard deviations calculated.\n";
	//Fills out pstd_dev
	catchdeviants(pstd_dev, pdeviantcount, pimages, x, y, files_size);
	//Fills out pdeviant_count
	CImg<unsigned char> time_lapse = normalized_std_dev_display(pstd_dev, pnormalized_std_dev, x, y);
	//Fills out pnormalized_std_dev & creates time-lapse object time_lapse
	//-----------------------------



	get_deviants(files_size,pdeviantcount,files, x, y, pimages,pstd_dev, paverage_by_pixel, invalids, angles, times, invalids_time, outputstring, dir);
	//Processes all images, creates log.txt, saves all processed images to "output" folder.



	//if ((int)invalids.size()>0) rectify_invalids(invalids, angles, times, invalids_time);
	//Gives user the chance to manually assign angles to invalid images.  Consider doing this before filling out log.txt

	ofstream histogram;
	string histostring = argv[1];
	histostring += ".txt";
	histostring.insert(0,"data/");
	char histochar[(int)histostring.length()];
	for (int i=0;i<(int)histostring.length();i++){
		histochar[i]=histostring[i];
	}
	histochar[(int)histostring.length()]='\0';
	histogram.open(histochar);
	generate_histogram(histogram, angles, times);
	histogram.close();
	cout << "Data exported.\n";
	//Creates a histogram by the name of the video file in the "data" folder of the imager directory.


	string histostring1 = dir + "/aime_lapse.png";
	char histochar1[(int)histostring1.length()];
	for (int i=0;i<(int)histostring1.length();i++){
		histochar1[i]=histostring1[i];
	}
	histochar1[(int)histostring1.length()]='\0';
	time_lapse.save_png(histochar1);
	cout << "Time lapse photo created.\n";
	//Saves time lapse photo
	

	delete[] pimages;
	delete[] pstd_dev;
	delete[] pnormalized_std_dev;
	delete[] pdeviantcount;
	delete[] paverage_by_pixel;
	return 0;
}




























/*void hoowoo (int r, CImg<unsigned char> temp){
	string str;
	stringstream ss;
	ss << r;
	str = ss.str();
	str.insert(0,"derpin/image");
	str.append(".jpg");
	if (r<10000){
		if (r<1000){
			if (r<100){
				if (r<10){
					char s[17];
					for (int j=0;j<17;j++){
						s[j]=str.at(j);
					}
					s[17]='\0';
					temp.save_jpeg(s);
				}
				else{
					char s[18];
					for (int j=0;j<18;j++){
						s[j]=str.at(j);
					}
					s[18]='\0';
					temp.save_jpeg(s);
				}
			}
			else{
				char s[19];
				for (int j=0;j<19;j++){
					s[j]=str.at(j);
				}
				s[19]='\0';
				temp.save_jpeg(s);
			}
		}
		else{
			char s[20];
			for (int j=0;j<20;j++){
				s[j]=str.at(j);
			}
			s[20]='\0';
			temp.save_jpeg(s);
		}
	}
}*/









