#include <stdio.h>
#include <stdlib.h>
#include "clUtils.h"
typedef struct{
cl_char letter;
cl_char[3];
cl_int index;
}container;

// What this does so far - loads the text string, puts that into the container along with its index, and creates the concatnated unsorted sample string R
// Note that R is not represented here as a trigram, so any sorting code would need to take that into account.
int main(int argc, char** argv){

int strLen = (int)fsize(argv[1]); // First cmdline arg should be the filename
char* inStr = (char*)cluFileExtractor(argv[1]); // Text string read from a text file is loaded into a buffer
container* src = (container*)malloc(sizeof(container)*strLen); // Creates the src container (i.e. T)
container* r1 = (container*)malloc(sizeof(container)*strLen);
container* r2 = (container*)malloc(sizeof(container)*strLen);
container* sampleSrc = (container*)malloc(sizeof(container)*strLen*2); //Creates the src container for the sample string (i.e., R)
int z = 0; //Dummy variable for properly loading sampleSrc

for(int i = 0; i<strLen; i++){ // Loads the text string and index into the src and sampleSrc container
	src[i].letter = inStr[i];
	src[i].index = i;
}
for(int i = 1; i<strLen; i++){
	r1[i-1].letter = src[i].letter;
	r1[i-1].index = i;
}
for(int i = 2; i<strLen; i++){
	r2[i-2].letter = src[i].letter;
	r2[i-2].index = i;
}
if(strLen mod 3 == 0){  //The idea behind this was to pad the end like we were supposed to. 
	r1[strLen-1].letter = '0';
	r2[strLen-2].letter = '0';
	r2[strLen-1].letter = '0';
}
if(strLen mod 3 == 1){
	r2[strLen-1].letter = '0';
}
if(strLen mod 3 == 2){
	r1[strLen-1].letter = '0';
	r1[strLen].letter = '0';
}
strcpy(sampleSrc,r1);
strcat(sampleSrc,r2);	

//How do we plan to sort this thing? I feel like using qsort would be kinda wierd. 
