#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include <chrono>
#include "image-seq.hpp"
#include "bmpImage.hpp"

using namespace std;

int gaussMask[5][5] = {1,  4,  7,  4, 1,
                       4, 16, 26, 16, 4,
                       7, 26, 41, 26, 7,
                       4, 16, 26, 16, 4,
                       1,  4,  7,  4, 1};

int gaussWeight = 273; 

int sobelMaskX[3][3] = {1,   2,  1,
                        0,   0,  0,
                       -1,  -2, -1};

int sobelMaskY[3][3] = {-1, 0, 1,
                        -2, 0, 2,
                        -1, 0, 1};

int sobelWeight = 8;

using clk = chrono::high_resolution_clock;



void compareImages(char* original, char* result) {
    bmpImage originalImage = createImage(original);
    bmpImage resultImage = createImage(result);
    originalImage.equals(resultImage);
}

unsigned char sobel(unsigned char* original, int col, int row, int color, int width, int height) {
    int pixelResultX = 0;
    int pixelResultY = 0;

    //This is just a codification of the formula given by the instructions (with a little modification to avoid an "out of bounds" exception)
    for(int s=-1; s<=1; s++){
        for(int t=-1; t<=1; t++){
            if(!((row + s)<=0 || (col + t)<=0 || (col + t)>= width || (row + s)>=height)){
                pixelResultX += (sobelMaskX[s+1][t+1] * (int) original[3 * (row + s) * width + (col + t) * 3 + color])/sobelWeight;
                pixelResultY += (sobelMaskY[s+1][t+1] * (int) original[3 * (row + s) * width + (col + t) * 3 + color])/sobelWeight;
            }
                
        }
    }

    return (unsigned char)(abs(pixelResultX) + abs(pixelResultY));
}

unsigned char gauss(unsigned char* original, int col, int row, int color, int width, int height) {
    int pixelResult = 0;

    //This is just a codification of the formula given by the instructions (with a little modification to avoid an "out of bounds" exception)
    for(int s=-2; s<=2; s++){
        for(int t=-2; t<=2; t++){
            if(!((row + s)<=0 || (col + t)<=0 || (col + t)>= width || (row + s)>=height))
                pixelResult += gaussMask[s+2][t+2] * (int) original[3 * (row + s) * width + (col + t) * 3 + color];
        }
    }

    return (unsigned char)(pixelResult/gaussWeight);
}

void applySobelFilter(unsigned char* original, unsigned char* result, int width, int height) {
    //We want to apply the corresponding formula to every color of every pixel of the image
    for(int row = 0; row < height; row++ ){
        for(int col = 0; col < width; col++){
            for(int color = 0; color < 3; color++)
                result[3 * row * width + col * 3 + color] = sobel(original, col, row, color, width, height);
        }
    }
}

void applyGaussFilter(unsigned char* original, unsigned char* result, int width, int height) {
    //We want to apply the corresponding formula to every color of every pixel of the image
    for(int row = 0; row < height; row++ ){
        for(int col = 0; col < width; col++){
            for(int color = 0; color < 3; color++){
                result[3 * row * width + col * 3 + color] = gauss(original, col, row, color, width, height);
            }
        }
    }
}

bool processImage(char* mode, char* source, char* destiny) {
    //Filter timestamps declarations
    chrono::microseconds gaussTime;
    chrono::microseconds sobelTime;

    //Total time starting point
    auto totalTimeStart = clk::now();

    //Load time starting point
    auto loadTimeStart = clk::now();


    //We first open the source file
    FILE* reader = fopen(source, "rb");
    if(reader == NULL){
        cerr << "Error: the image file doesn't exist or can't be opended.\n";
        return false;
    }

    bmpImage image;
    //Now, we read the header information
    if(fread(image.info, sizeof(unsigned char), 54, reader) < 54) {
        cerr << "Error: the header file could not be readed.\n";
        return false;
    }
    

    //We check if the image meets the program's requirements
    if(!image.checkImageRequirements()) return false;

    //Now, we can start writing the header information
    FILE* writer = fopen(destiny, "wb");
    if(writer == NULL){
        cerr << "Error: the new image file couldn't be created.\n";
        return false;
    }
    fwrite(image.info, sizeof(unsigned char), 54, writer);


    //Finally, we can read all of the data, first of all we take both the width and the height
    int width, height;
    mempcpy(&width, image.info + 18, sizeof(int));
    mempcpy(&height, image.info + 22, sizeof(int));

    //With those two values, we can now compute the size for the data vector
    int size = 3 * width * abs(height);
    image.data = new unsigned char[size];
    //unsigned char* data = new unsigned char[size];

    //We move both reader and writer to the data sections
    int dataOffSet;
    mempcpy(&dataOffSet, image.info + 10, sizeof(int));
    fseek(reader, dataOffSet, SEEK_SET);
    fseek(writer, dataOffSet, SEEK_SET);
    
    //And then, we read all of the image data in the BGR format
    if(fread(image.data, sizeof(unsigned char), size, reader) != (size_t)size) {
        cerr << "Error: the data could not be readed.\n";
        return false;
    }
    



    //Load time ending point
    auto loadTimeEnd = clk::now();

    //Store time starting point
    auto storeTimeStart = clk::now();



    //If we are on copy mode, we just write all of the original data into the new directory
    if(strcmp(mode, "copy") == 0){
        fwrite(image.data, sizeof(unsigned char), size, writer);
        delete(image.data);
    }  

    //Otherwise, we create new Data and then write it into the new directory
    else {
        unsigned char* gaussData = new unsigned char[size];

        //Gauss time starting point
        auto gaussTimeStart = clk::now();

        applyGaussFilter(image.data, gaussData, width, height);

        //Gauss time ending point
        auto gaussTimeEnd = clk::now();
        gaussTime = chrono::duration_cast<chrono::microseconds> (gaussTimeEnd - gaussTimeStart);

        if(strcmp(mode, "sobel") == 0) {
            unsigned char* sobelData = new unsigned char[size];

            //Sobel time starting point
            auto sobelTimeStart = clk::now();

            applySobelFilter(gaussData, sobelData, width, height);

            //Sobel time ending point
            auto sobelTimeEnd = clk::now();
            sobelTime = chrono::duration_cast<chrono::microseconds> (sobelTimeEnd - sobelTimeStart);

            storeTimeStart = clk::now();

            fwrite(sobelData, sizeof(unsigned char), size, writer);
            delete(sobelData);
        }

        else {
            storeTimeStart = clk::now();

            fwrite(gaussData, sizeof(unsigned char), size, writer);
            delete(gaussData);
        }
        
    }

    //Store time ending point
    auto storeTimeEnd = clk::now();

    //Total time ending point
    auto totalTimeEnd = clk::now();


    //Finally, we calculate the general timestamps
    auto totalTime = chrono::duration_cast<chrono::microseconds> (totalTimeEnd - totalTimeStart);
    auto loadTime = chrono::duration_cast<chrono::microseconds> (loadTimeEnd - loadTimeStart);
    auto storeTime = chrono::duration_cast<chrono::microseconds> (storeTimeEnd - storeTimeStart);

    //Print timestamps
    cout << "File: " << source << " (time: " << totalTime.count() << ")\n";
    cout << "   Load time: " << loadTime.count() << "\n";
    if(strcmp(mode, "copy") != 0){
        cout << "   Gauss time: " << gaussTime.count() << "\n";
        if(strcmp(mode, "sobel") == 0)
            cout << "   Sobel time: " << sobelTime.count() << "\n";
    }
    cout << "   Store time: " << storeTime.count() << "\n";
    
    fclose(reader);
    fclose(writer);

    return true;
}

bool processDirectoryFiles(char* mode, char *sourcePath, char *destinyPath) {
    struct dirent *entry;
    DIR *sourceDirectory;
    DIR *destinyDirectory;

    //First of all, we check that both directories are correct
    if((sourceDirectory = checkDirectory(sourcePath, "entrada")) == NULL || (destinyDirectory = checkDirectory(destinyPath, "salida")) == NULL)
        return false;

    //Print information about directories
    cout << "Input path: " << sourcePath << "\n";
    cout << "Output path: " << destinyPath << "\n";

    //Now, we read each file of the directory
    while ((entry = readdir(sourceDirectory)) != NULL) {
        //For every non-system file, we create both an entry file path and a destiny file path
        if ((strcmp(entry->d_name, ".") * strcmp(entry->d_name, "..")) != 0) {
            string sourceFilePath = sourcePath;
            sourceFilePath.append("/");
            sourceFilePath.append(entry->d_name);

            string destinyFilePath = destinyPath;
            destinyFilePath.append("/");
            destinyFilePath.append(entry->d_name);

            //Finally, we process the image according to the execution mode
            if(!processImage(mode, &sourceFilePath[0], &destinyFilePath[0])) return false;
        }
    }
    
    delete(entry);
    closedir(sourceDirectory);
    closedir(destinyDirectory);

    return true;
}

bool checkNumberOfArguments (int arguments, int exceptedNumberOfArguments) {
    if(arguments != exceptedNumberOfArguments) {
        cerr << "Error: Numero de parametros incorrecto" << "\n";
        cout << "Formato: ./img-seq modo dir_entrada dir_salida" << "\n";
        cout << "modo: copy, gauss, sobel" << "\n";

        return false;
    }
    return true;
}

bool checkMode(char* mode) {
    if(!(strcmp(mode, "copy") == 0 || strcmp(mode, "gauss") == 0 || strcmp(mode, "sobel") == 0)) {
        cerr << "Error: Modo de operacion no valido" << "\n";
        cout << "Formato: ./img-seq modo dir_entrada dir_salida" << "\n";
        cout << "modo: copy, gauss, sobel" << "\n";

        return false;
    }

    return true;
}

DIR* checkDirectory(char* directoryPath, string directoryDirection) {
    DIR *directory;

    if((directory = opendir(directoryPath)) == NULL) {
        cerr << "Error: el directorio de " << directoryDirection << " no existe o no se puede abrir" << "\n";
        return NULL;
    }

    return directory;
}



/* *
 * Main execution of the programm, this programm applys one of three mode to all of the images on one directory and places the results
 * in other directory.
 * 
 * The modes available are copy, which copies all of the images, gauss, which applys a gaussian blurr to all of the images,
 * and sobel, which marks up all of the borders of the image.
 * 
 * @param args number of arguments of the function call
 * @param argv[] arguments of the function, expected format: ./image-seq mode dir_in dir_out
 * @return 0 if correct execution and -1 if error occured
 * */
int main(int args, char* argv[]) {
    //We first check if the arguments are correct
    if(!checkNumberOfArguments(args, 4) || !checkMode(argv[1]))
        return -1;

    //Now, we process every element from the source directory to the destiny directory
    if(!(processDirectoryFiles(argv[1], argv[2], argv[3])))
        return -1;

    return 0;
}