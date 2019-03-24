/*
  Compile with 'g++ -o Name Name.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11'
  The program decrypts a message inside a given image at a given channel (r, g, b).
*/
#include <string.h>
#include <string>
#include <stdio.h>
#include "CImg.h"

using namespace cimg_library;

FILE *FOut;

//This function helps to print every single bit of a variable given the size in bytes.
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

int main(int argc, char *argv[]) {
  
	if(argc < 4){
		printf("Usage: %s <image> <file to write> <channel corrupted>\n", argv[0]);
		exit(0);
	}
	
	FOut = fopen(argv[2], "w");   //Open the file to write the final decrypted message.
	
	unsigned short channel = atoi(argv[3]);   //Set the channel where read the message.
	
  CImg<unsigned char> image(argv[1]);       //Load the encrypted image.
	 
	short shift = 0;        //Which bit to take in consideration.
	char chara = 0;     //The chara to decrypt.
	
	for( int w = 0; w < image.width(); ++w ){
		for( int h = 0; h < image.height(); ++h ){
			if(!(chara == '\0' && shift == 8)){     //If we haven't reached the null char yet
				
        printf("-=--=--=--=--=--=--=--=--=--=--=--=--=-\n");
        
        if(shift == 8){	                          //If we have all the 8 bits we can print next char.
					printf("[DEBUG] built the char %c\n", chara);
					fprintf(FOut, "%c", chara);
					chara = 0;
				}
			
				shift = shift % 8;

				unsigned char *c = image.data(w, h, 0, channel);    //Read the value of the pixel.

				printf("[DEBUG] image(%d, %d, 0, %d) = ", w, h, channel); 
				printBits(sizeof(unsigned char), c);
				
				unsigned char flag = *c & 1;    //Take the first bit of the pixel.
	
				chara = chara | flag << shift;    //Put the bit in the char at shift position.
				printf("\tchar bits = ");
				printBits(1, &chara);
			
				++shift;
        
        printf("-=--=--=--=--=--=--=--=--=--=--=--=--=-\n");
			}
		}
	}
	

	printf("\n[UPDATE] Secret message succesfully read in the image\n");
	
	fprintf(FOut, "\n");
	return 0;
}


