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
#include <sqlite3.h>
#include <stdio.h>
using namespace cimg_library;
using namespace std;

#define PI 3.14159265

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



static int callback (void *Empty, int argc, char **argv, char **azColName){
	for (int i=0;i<argc;i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//Needed for sqlite3 stuff



int main(int argc, char *argv[]){


	if (argc == 1){
		cout << "Enter './run.sh' and make sure there is at least one .wmv file in this directory and that its corresponding folder (if any) has been removed.\n";
		return 0;
	}
	//If imager is run from command line instead of run.sh or if there are no .wmv files in the directory.


	vector<string> files = vector<string>();
	string dir = argv[1];
	getdir(dir,files);
	//See getdir comments above

	/*string outputstring = dir + "/output";
	char outputchar[(int)outputstring.length()];
	for (int i=0;i<(int)outputstring.length();i++){
		outputchar[i]=outputstring[i];
	}
	outputchar[(int)outputstring.length()]='\0';
	mkdir (outputchar, 0777);*/
	//Makes directory within image directory called "output"

	//CREATING TABLE:
	//Getting table name:
	string dir_excised = dir;
	dir_excised.erase(0,22);
	char table_name[(int)dir_excised.length()]; //Alright, this part needs to become a character variable indicating the specific date AND speed
	table_name[0]='d';
	for (int i=1;i<(int)dir.length();i++){
		if (dir[i] == '-'){
			continue;
		}
		table_name[i]=dir[i];
	}
	table_name[(int)dir.length()]='\0';

	//Opening sqlite3 connection:
	/*sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(table_name, &db);
	if ( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return (1);
	}*/
	//Problems converting char to const char to open sql connection
	
	//Orders "files list:
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

	CImg<unsigned char> temp_image;
	temp_image.load_jpeg("/home/randylab/imager/angletest/image1.jpg");
	int x = (int)temp_image.width(); //640
	int y = (int)temp_image.height(); //480

	ofstream sql_command;
	sql_command.open("sql_command.txt");
	sql_command << "CREATE TABLE " << table_name << " (image_number double";
	for (int j=0;j<y;j++){
		for (int i=0;i<x;i++){
			rc = sqlite3_exec(db
			sql_command << ", p" << j*640+i << " varchar(10)";
		}
	}
	sql_command << ");";
	sql_command.close();
	cout << "\nSQL Command generated." << endl;


	//Executing create table command
	//rc = sqlite3_exec(db, [sql command], callback, 0, zErrMsg)
	
	//sqlite3_close(db);

	/*for (int i=0;i<files_size;i++){
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
						//First create table based on todays date with values (P1 int, P2 int, etc)
							//script this to a file: create header ("create table %s (image_number int", date variable)
							//for i in rows, j in columns, for the picture, print on same row ", P[i*640+j] int"
							//end with footer.
						//SQLite call to create that table;

						//For each image:
						//load image
						//do pixel stuff; free image

						//For each pixel:
						//calculate intensity via intensity function call
						// "insert into 07_31_12 (P[i*640+j]) values ([pixel intensity])"
						//Do this for each image, each row being a different image
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
	}*/
	return 0;
}
