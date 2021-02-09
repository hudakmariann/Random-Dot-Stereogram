#include "BaseImage.h"
#include <cmath>
#include <iostream>


void BaseImage:: calcBaseParams()
{
	calcDisplaySizeMM();
	calcDisplayHeightMM();
	calcDisplayWidthMM();
	calcPixelSize();
	calcJuleszPixSize();
	calcHorMidLine();
	calcVertMidLine();
}


void BaseImage::calcDisplaySizeMM()
{
	displaysizeMM_ = displaysizeInch_ * 25.4;
}

void BaseImage::calcDisplayHeightMM()
{
	int const power = 2;
	try
	{
		if ((displayheight_ == 0)) 
			throw std::runtime_error("ERROR: Division by zero in void BaseImage::calcDisplayHeightMM()!");
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
	}
	float imageproportion = float(displaywidth_) / float(displayheight_); 

	//std::cout << "calcDisplayHeightMM = " << sqrt((pow(displaysizeMM_, power)) / (pow(imageproportion, power) )) << std::endl;


	try
	{
		if ((imageproportion == 0))
			throw std::runtime_error("ERROR: Division by zero in void BaseImage::calcDisplayHeightMM()!");
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
	}
	displayheightMM_= sqrt((pow(displaysizeMM_, power)) / (pow(imageproportion, power) ));
}

void BaseImage::calcDisplayWidthMM()
{
	int const power = 2;
	//std::cout << "calcDisplayWidthMM = " << sqrt((pow(displaysizeMM_, power) - pow(displayheightMM_, power))) << std::endl;
	displaywidthMM_= sqrt((pow(displaysizeMM_, power) - pow(displayheightMM_, power)));
}

void BaseImage::calcPixelSize()
{
	try
	{
		if ((displaywidth_ == 0))
			throw std::runtime_error("ERROR: Division by zero in void BaseImage::calcPixelSize()!");
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
	}

	pixelsize_ = displaywidthMM_ / displaywidth_;

	try
	{
		if ((pixelsize_ == 0))
			//later many divisions take place with pixelsize_, so it is useful to catch this as early as possible
			throw std::runtime_error("ERROR: pixelsize_= 0! Risk of division by zero!");  
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
	}
	//std::cout << "calcPixelSizeH = " << (displayheightMM_ / displayheight_) << std::endl;
	//std::cout << "calcPixelSizeW = " << (displaywidthMM_ / displaywidth_) << std::endl;
}

void BaseImage::calcHorMidLine()
{
	hormidline_=  (displayheight_/2) - (((displayheight_ / 2)) % juleszpixsizePX_); //juleszpixsizePX_ cannot be zero, since it is assigned 1 in case of 0 when it is calculated.
}

void BaseImage::calcVertMidLine()
{
	vertmidline_= (displaywidth_ / 2)- (((displaywidth_ / 2))%juleszpixsizePX_);
}

void BaseImage::calcJuleszPixSize()
{
	juleszpixsizePX_ = roundf((juleszpixsizeMM_/10) / (pixelsize_));    //"/10" is because the user provides data in 1/10 mm in the input file

	//std::cout << "BaseImage::JuleszPixSizeMM = " << juleszpixsizeMM_ << std::endl;
	//std::cout << "BaseImage::calcJuleszPixSizePX = " << juleszpixsizePX_<< std::endl;

	if (juleszpixsizePX_ == 0) juleszpixsizePX_ = 1;
	std::cout << "calcJuleszPixSizePX = " << juleszpixsizePX_ << std::endl;
}


int BaseImage::GetJuleszPixSizePX_()
{
	return juleszpixsizePX_;
}

int  BaseImage::GetDisplayWidth()
{
	return displaywidth_;
}

int  BaseImage::GetDisplayHeight()
{
	return displayheight_;
}



