#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include "bmpImage.hpp"

using namespace std;

bool bmpImage::checkImageRequirements() {
    //Let's check that the file we are trying to read is actually a bmp image
    short fileType;
    memcpy(&fileType, info, sizeof(short));
    if(fileType != 0x4D42 /* 0x4D42 <=> BM */) {
        cerr << "Error: the image file type is not bmp.\n";
        return false;
    }

    //We also need to check if the image meets the instructions' requirements
    short planes, pointSize;
    int compression;
    mempcpy(&planes, info + 26, sizeof(short));
    mempcpy(&pointSize, info + 28, sizeof(short));
    mempcpy(&compression, info + 30, sizeof(int));

    if(planes != 1 || pointSize != 24 || compression != 0){
        cerr << "Error: the image doesn't meet this program's requirements\n";
        cout << "Number of planes: Expected = 1 | Actual = " << planes << "\n";
        cout << "Point size: Expected = 24 | Actual = " << pointSize << "\n";
        cout << "Compression: Expected = 0 | Actual = " << compression << "\n";
        return false;
    }

    return true;
}

bool bmpImage::equals(bmpImage image) {
    bool result = true;
    int i;
    for (i = 0; i<(int)sizeof(data)-3 && result; i+=3)
        result = (data[i] = image.data[i]) && (data[i+1] = image.data[i+1]) && (data[i+2] = image.data[i+2]);

    if(result) cout << "Both images' data are equals" << "\n";

    else{
        cout << "Both images' data are not equals" << "\n";
        cout << "The first differing pixel is: " << i/3 << "\n";

        cout << "Image A -> "; printPixelInfo(i/3);

        cout << "Image B -> Information of pixel " << i/3 << ": (B, G, R) = (" <<
        (int) image.data[i] << ", " << 
        (int) image.data[i+1] << ", " << 
        (int) image.data[i+2] << ")\n";
    }  

    return result;
}

void bmpImage::printPixelInfo(int i) {
    cout << "Information of pixel " << i << ": (B, G, R) = (" <<
    (int) data[i] << ", " << 
    (int) data[i+1] << ", " << 
    (int) data[i+2] << ")\n";
}

void bmpImage::printInfo() {
    short fileType, planes, pointSize;
    mempcpy(&fileType, info, sizeof(short));
    mempcpy(&planes, info + 26, sizeof(short));
    mempcpy(&pointSize, info + 28, sizeof(short));

    int fileSize, dataStartingPoint, width, height, compression,
    imageSize, horizontalResolution, verticalResolution, colorTableSize, importantColors;
    mempcpy(&fileSize, info + 2, sizeof(int));
    mempcpy(&dataStartingPoint, info + 10, sizeof(int));
    mempcpy(&width, info + 18, sizeof(int));
    mempcpy(&height, info + 22, sizeof(int));
    mempcpy(&compression, info + 30, sizeof(int));
    mempcpy(&imageSize, info + 34, sizeof(int));
    mempcpy(&horizontalResolution, info + 38, sizeof(int));
    mempcpy(&verticalResolution, info + 42, sizeof(int));
    mempcpy(&colorTableSize, info + 46, sizeof(int));
    mempcpy(&importantColors, info + 50, sizeof(int));

    cout << "BMP IMAGE INFORMATION\n";
    cout << "\n";
    cout << "File Type: " << fileType <<"\n";
    cout << "Planes: " << planes <<"\n";
    cout << "pointSize: " << pointSize <<"\n";
    cout << "fileSize: " << fileSize <<"\n";
    cout << "dataStartingPoint: " << dataStartingPoint <<"\n";
    cout << "width: " << width <<"\n";
    cout << "height: " << height <<"\n";
    cout << "compression: " << compression <<"\n";
    cout << "imageSize: " << imageSize <<"\n";
    cout << "horizontalResolution: " << horizontalResolution <<"\n";
    cout << "verticalResolution: " << verticalResolution <<"\n";
    cout << "colorTableSize: " << colorTableSize <<"\n";
    cout << "importantColors: " << importantColors <<"\n";
}

bmpImage createImage(char* file){
    FILE* reader = fopen(file, "rb");

    bmpImage image;
    if(fread(image.info, sizeof(unsigned char), 54, reader) < 54)
        cerr << "Error: the header file could not be readed.\n";
    

    int width, height;
    mempcpy(&width, image.info + 18, sizeof(int));
    mempcpy(&height, image.info + 22, sizeof(int));

    int size = 3 * width * abs(height);
    image.data = new unsigned char[size];

    //We move both reader and writer to the data sections
    int dataOffSet;
    mempcpy(&dataOffSet, image.info + 10, sizeof(int));
    fseek(reader, dataOffSet, SEEK_SET);
    
    //And then, we read all of the image data in the BGR format
    if(fread(image.data, sizeof(unsigned char), size, reader) != (size_t)size)
        cerr << "Error: the data could not be readed.\n";

    fclose(reader);

    return image;
}

