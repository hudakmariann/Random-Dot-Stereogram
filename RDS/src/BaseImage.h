#ifndef BaseImage_h
#define BaseImage_h


#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include <memory>
#include <iostream>


class BaseImage
{

public:
    
    int GetDisplayWidth(); //returns the display width in pixels
    int GetDisplayHeight(); //returns the display height in pixels
    int GetJuleszPixSizePX_(); //returns the size of a random dot in pixels
    cv::Mat baseImg; //the base image is a black-white image, that serves as a source image for creating the random dot stereogram in RDS.cpp

    
private:
    int const backgroundclr_ =255; //greyscale  value of the base image background
    int const shapeclr_ = 0; //greyscale  value of the base image shapes

   
    void calcDisplaySizeMM();  //calculates the display size from inch to mm
    void calcDisplayHeightMM(); //calculates the disp. height in  mm
    void calcDisplayWidthMM(); //calculates the disp. width in  mm
    void calcPixelSize(); //calculates the size of each pixels in the given display
    void calcHorMidLine(); //calculates the horizontal midline of the display
    void calcVertMidLine(); //calculates the vertical midline of display
    void calcJuleszPixSize(); //calculates the size of each random dot in pixels
 

protected:

    void calcBaseParams(); //calls all calculation functions of this class

    float displaysizeInch_; //these are read from the input file
    float displaysizeMM_;
    float displayheightMM_;
    float displaywidthMM_;
  
    int displaywidth_; //read from file, pixels, this is the cv::Mat image size
    int displayheight_; //read from file, pixels, this is the cv::Mat image size
    int imageproportion_; //calculated from the width and height of the display
    float pixelsize_;      //the real size of the pixels of the given display
    int vertmidline_, hormidline_;    //kép középvonalai

    float juleszpixsizeMM_;  //size of the random dots given in the input file in 1/10 mm
    int juleszpixsizePX_; //size of the random dots in pixels, calculated 

};

#endif