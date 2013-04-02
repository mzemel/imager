#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

#define base 0x378
#define value 255

int main(){
	if (ioperm(base,1,1)){
		fprintf(stderr, "Couldn't work");
	}
	cout << "Data pins 5 and 8 set to 0, 10v chip activated\n";
	outb(111,base);
	return 0;
}
