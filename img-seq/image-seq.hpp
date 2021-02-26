#ifndef MAIN_SEQ_HEADER_FILE
#define MAIN_SEQ_HEADER_FILE

#include <vector>
#include <string>
#include <iostream>
#include <string.h>
#include <dirent.h>

using namespace std;

/**
 * Creates and compares two images, showing wheter or not they are equal to each other
 * 
 * This function creates one image from each argument path and then invokes the equals method in one to the other,
 * showing detailed information about the execution
 * 
 * @param original path to the original file
 * @param result path to the result file
 * @return void
 * */
void compareImages(char* original, char* result);

/**
 * Applies a sobel filter to a color value of a pixel
 * 
 * This function just applies the mathematical sobel formula to a pixel and returns the result.
 * 
 * @param original data of the original image
 * @param col column of the pixel to modify
 * @param row row of the pixel to modify
 * @param color color of the pixel to modify
 * @param width width of the image
 * @param height height of the image
 * @return the resulting pixel of the filter application
 * */
unsigned char sobel(unsigned char* original, int col, int row, int color, int width, int height);

/**
 * Applies a gauss blur to a color value of a pixel
 * 
 * This function just applies the mathematical gauss formula to a pixel and returns the result.
 * 
 * @param original data of the original image
 * @param col column of the pixel to modify
 * @param row row of the pixel to modify
 * @param color color of the pixel to modify
 * @param width width of the image
 * @param height height of the image
 * @return the resulting pixel of the filter application
 * */
unsigned char gauss(unsigned char* original, int col, int row, int color, int width, int height);

/**
 * Applies a sobel filter to an image
 * 
 * This function access to every color of every pixel of an image data and generates a new value for it by applying a sobel filter.
 * 
 * @param original data of the original image (with gauss filter already applied)
 * @param result data of the result image
 * @param width width of both images
 * @param height height of both images
 * @return void
 * */
void applySobelFilter(unsigned char* original, unsigned char* result, int width, int height);

/**
 * Applies a filter to an image
 * 
 * This function access to every color of every pixel of an image data and generates a new value for it by applying either a gauss filter.
 * 
 * @param original data of the original image
 * @param result data of the result image
 * @param width width of both images
 * @param height height of both images
 * @return void
 * */
void applyGaussFilter(unsigned char* original, unsigned char* result, int width, int height);

/**
 * Process an image and stores the result
 * 
 * This function manage both files with two FILE* variables with which reads, optionally process,
 * and writes all of the file content from the source path to the destiny path
 * 
 * @param mode Execution mode for processing the images = {"copy", "gauss", "sobel"}
 * @param source Path of the source file to copy
 * @param destiny Path of the destiny to copy the file to
 * @return true if there were no errors and false if there was an error
 * */
bool processImage(char* mode, char* source, char* destiny);

/**
 * Process all of the files from one directory to another
 * 
 * This function creates two DIR* variables to manage both directories, then checks if those paths are correct.
 * Afterwards, gets all of the entries of the source directory and create both the source and destiny path for each file, then process
 * those two paths to the corresponding method, according to the mode argument
 * 
 * @param mode Execution mode for processing the images = {"copy", "gauss", "sobel"}
 * @param sourcePath Path of the source directory
 * @param destinyPath Path of the destiny directory
 * @return true if there were no errors and false if there was an error
 * */
bool processDirectoryFiles(char* mode, char *sourcePath, char *destinyPath);

/**
 * Check if the number of arguments is correct.
 * 
 * This function just compares the number of the two arguments and return the corresponding value.
 * It also notifies the programm through cerr of the possible error.
 * 
 * @param arguments the arguments of the main function execution
 * @param exceptedNumberOfArguments the  number of arguments expected for this especific programm
 * @return true if the number of arguments is correct and false if it is not
 * */
bool checkNumberOfArguments (int arguments, int exceptedNumberOfArguments);

/**
 * Check if the mode of execution is valid.
 * 
 * This function compare the mode introduced by the user and all of the possible modes of execution for the programm
 * and notifies if it is not one of them through cerr and cout.
 * 
 * @param mode the mode that the user has introduced on the execution
 * @return true if the mode is one of the options available and false if it is not
 * */
bool checkMode(char* mode);

/**
 * Check if the directory exists and can be opened.
 * 
 * This function tries to open a directory through opendir() and checks that the value returned is correct.
 * If not, notifies the user of a possible error through cerrr and cout.
 * 
 * @param directoryPath path of the directory to open
 * @param directoryDirection word representing whether the directiory is an input or an output directory. Possible values = {"entrada", "salida"}
 * @return directory, a DIR* variable with the starting point of the directory if it could be opened and NULL if it couldn't or if it didn't exist.
 * */
DIR* checkDirectory(char* directoryPath, string directoryDirection);

#endif