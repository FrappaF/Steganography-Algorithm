/*
  Compile with 'g++ -o Name Name.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11'
  The program encrypts a message inside a given image at a given channel (r, g, b).
*/
#include <string.h>
#include <stdio.h>
#include "CImg.h"

using namespace cimg_library;

#define MASK 254   //Mask to take last bit of a character.

//This function helps to print every single bit of a variable given the size in bytes.
void printBits(size_t const size, void const * const ptr){
  
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
  
	if(argc < 5){
		printf("Usage: %s <image> <secret message> <channel to corrupt> <name of the encrypted image>\n", argv[0]);
		exit(0);
	}
	
	short channel = atoi(argv[3]);  //Which channel to encrypt (0 for red, 1 for green, 2 for blue).
	char *message = argv[2];        //The message to encrypt.
	
  	CImg<unsigned char> image(argv[1]), secret(argv[1]);    //Read the image
	

  	short shift = 0;    //Which bit to take in consideration.
	int index = 0;      //Which character to take in consideration.
	char character;     //The character to encrypt.

	
	for( int w = 0; w < image.width(); ++w ){
		for( int h = 0; h < image.height(); ++h ){
			if(index < strlen(message)+1 || shift != 8){    //If we can encrypt another character of the message
				                                              //or we have to encrypt the null character we can go.  

        			if(index == strlen(message) && shift == 8){   //IF we are at the end of the message we put the null character.   
					character = '\0'; 
					++index;
				}
				
				shift = shift % 8;                          

				if(shift == 0 && index < strlen(message)){	  //Load next character.
					character = message[index++];
				}

				printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				printf("[DEBUG] character = %c shift = %d\n", character, shift);
				unsigned char *c = image.data(w, h, 0, channel);    //Original value of the pixel.
				printf("[DEBUG] image(%d, %d, 0, %d) = ", w, h, channel);
				printBits(sizeof(unsigned char), c);

				unsigned char flag = (character & (1 << shift));    //Obtain the bit of the character we want.
				flag >>= shift;                                     //Putting at the first position so we can have 1 or 0
				flag = flag | MASK;                                 //Creating the mask (flag can be 254 if last bit was 0 or 255 if the last bit was 1). 

				printf("\tflag = %d\n", flag);

				if(*secret.data(w, h, 0, channel) % 2 == 0){       //If the pixel has 0 as first bit
					if(flag % 2 != 0){                               //Change the value if flag is 255.
						secret(w, h, 0, channel) = *secret.data(w, h, 0, channel) + 1;	
					}
				}
				else{                                               //Otherwise if the pixel has 1 as first bit
					if(flag % 2 == 0){                                //Change if flag is 254.
						secret(w, h, 0, channel) = *secret.data(w, h, 0, channel) - 1;	
					}
				}

				printf("[DEBUG] secret(%d, %d, 0, %d) = ", w, h, channel);
				printBits(1, &secret(w, h, 0, channel));

					++shift;
				printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
			}
		}
	}
	
	//secret.display(secret_disp);
	//image.display(original_disp);
	
	secret.save_png(argv[4]);    //Save the encrypted image.
	
	printf("\n[UPDATE] Secret message succesfully wrote in the image\n");
	
	//while(!original_disp.is_closed() && !secret_disp.is_closed()){}
	return 0;
}

