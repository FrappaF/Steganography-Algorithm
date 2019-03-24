# Steganography-Algorithm
To compile the source code use:
  g++ -o Name Name.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
  

EncryptImage.cpp create an image with a secret message inside.
It divides every single char of the message in 8 bits and put every single bit in the first position of the pixels of the image in a given channel (rgb).

DecryptImage.cpp takes an image an build a message picking the first bit of the pixels in a given channel (rgb) until it reaches a null char ('\0').
