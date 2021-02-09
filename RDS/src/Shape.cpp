#include "Shape.h"
#include <iostream>


void Shape::calcShapeParams()
{
	calcShapeSizePX();
	calcShapeDistance();

	calcHorDisparPX();
	calcVerDisparPX();
	calcCoordinates();
}


void Shape::calcShapeDistance()
{
	shapedistance = 3 * shapewidthPX ;
}

void Shape::calcCoordinates()
{
	//the modulo of the division by juleszpixsizePX_ is substracted from each coordinate so that the edges of the random dots of the background should be perfectly aligned with the random dots of the shapes
	 X1L  = (vertmidline_ - shapewidthPX - hordisparPX) + ((vertmidline_ - shapewidthPX - hordisparPX) % juleszpixsizePX_);
	 X1R  = (vertmidline_ - shapewidthPX + hordisparPX) - ((vertmidline_ - shapewidthPX + hordisparPX) % juleszpixsizePX_);
	 Y1L  = hormidline_ - shapeheightPX + verdisparPX + (displayheight_ % juleszpixsizePX_);
	 Y1R  = hormidline_ - shapeheightPX - verdisparPX + (displayheight_ % juleszpixsizePX_);
	 X2L  = (vertmidline_ + shapewidthPX - hordisparPX) +  ((vertmidline_ + shapewidthPX - hordisparPX) % juleszpixsizePX_);
	 X2R  = (vertmidline_ + shapewidthPX + hordisparPX) -  ((vertmidline_ + shapewidthPX + hordisparPX) % juleszpixsizePX_);
	 Y2L  = hormidline_ + shapeheightPX + verdisparPX - (displayheight_ % juleszpixsizePX_);
	 Y2R  = hormidline_ + shapeheightPX - verdisparPX - (displayheight_ % juleszpixsizePX_);
	 //std::cout << "shapewidthPX = " << shapewidthPX << std::endl; //cout for debugging
	 //std::cout << "hordisparPX = " << hordisparPX << std::endl;
	 //std::cout << "verdisparPX = " << verdisparPX << std::endl;
}

void Shape::calcHorDisparPX()
{
	//here the modulo is not substracted, since it is taken into account elsewhere
	hordisparPX = int(trunc(((hordisparMM / 10) / pixelsize_) / 2)); // the /10 is because values are given in 1/10 mm, so 1 in the input file stands for 0.1 mm
}

void Shape::calcVerDisparPX()
{
	verdisparPX = int(trunc(((verdisparMM / 10) / pixelsize_) / 2));
}

void Shape::calcShapeSizePX()
{
	int shpwPX = roundf((shapewidthMM / 2) / pixelsize_);
	shapewidthPX = shpwPX - (shpwPX % juleszpixsizePX_);  //modulo is substracted for random dots to be alighned in the shapes and background
	shapeheightPX = shapewidthPX; //it turned out to be superfluous to vary the shape image proportion
}

int Shape::GetShapeWidthPX()
{
	return shapewidthPX;
}

int Shape::GetShapeHeightPX()
{
	return shapeheightPX;
}

int Shape::GetShapeDistance()
{
	return shapedistance;
}