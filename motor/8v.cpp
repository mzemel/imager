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
	cout << "Data pins 3 and 8 set to 0, 8v chip activated\n";
	outb(123,base);
	return 0;
}
