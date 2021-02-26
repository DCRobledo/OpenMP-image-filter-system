#ifndef BMP_IMAGE_HEADER_FILE
#define BMP_IMAGE_HEADER_FILE

#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <dirent.h>
#include <fstream>

using namespace std;
/**
 * @brief Struct used to store a bmp image file information and data
 * */
struct bmpImage {
    unsigned char info[54]; // Header information
    unsigned char* data; // Pixel data in BGR format

    /**
     * Checks if the image meets this specific program's requirements
     * 
     * This function checks that the image is of type bmp and that the attributes planes, pointSize and compresion
     * have the correct values.
     * 
     * @return true if there were no errors and false if there was an error
     * */
    bool checkImageRequirements();

    /**
     * Compares the invoker image with another one
     * 
     * This function checks every byte of the two data section and notifies whether or not they are equal,
     * if they are not, it prints the first differing pixel and its information
     * 
     * @param image image to compare the invoker with
     * @return true if there were no errors and false if there was an error
     * */
    bool equals(bmpImage image);

    /**
     * Prints the information of a pixel in BGR format
     * 
     * This function just gets the values of the i pixel in the data section
     * 
     * @param i index of the pixel to print the information of
     * @return void
     * */
    void printPixelInfo(int i);

    /**
     * Prints the information of the image's header
     * 
     * This function just gets all of the header information and print it in an understable and visual way
     * 
     * @return void
     * */
    void printInfo();
};

/**
 * Creates an image from a path
 * 
 * This function reads the content of a file and creates a bmpImage struct to store all of the information and data
 * 
 * @param file path of the file
 * @return the resulting bmpImage struct
 * */
bmpImage createImage(char* file);

#endif