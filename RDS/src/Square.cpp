#include "Square.h"
#include <cmath>

using namespace cv;


Square::Square(const float& DisplaySize, const int& DisplayWidth, const int& DisplayHeight, const float& JuleszPixSize,
	const float& ShapeWidth, const float& ShapeHeight, const float& TotalhorizontalMM, const float& TotalVerticalMM,
	float SquareDisparMM, int orderSquare) : 	disparMM_(SquareDisparMM)
{
	displaysizeInch_ = DisplaySize;
	displaywidth_ = DisplayWidth;
	displayheight_ = DisplayHeight;
	juleszpixsizeMM_ = JuleszPixSize;
	shapewidthMM = ShapeWidth;
	shapeheightMM = ShapeHeight;
	hordisparMM = TotalhorizontalMM;
	verdisparMM = TotalVerticalMM;
	order = Square::position(orderSquare);
}

void Square::calcAllParams()
{
	calcBaseParams();
	calcShapeParams();
	calcDisparPX();

}

void Square::calcDisparPX()
{
	std::cout << "juleszpixsizePX in square = " << juleszpixsizePX_ << std::endl;
	std::cout << "pixsize in square = " << pixelsize_ << std::endl;
	std::cout << "disparMM in square = " << disparMM_ << std::endl;
	
	disparPX_ = (int(trunc((disparMM_/10) / pixelsize_) / 2) - (int(trunc((disparMM_/10) / pixelsize_) / 2)) % juleszpixsizePX_);
	
	//std::cout << "disparPX_ in square = " << disparPX_ << std::endl;
}	

int Square::GetDisparPX()
{
	return disparPX_;
}

void Square::Draw(std::shared_ptr<cv::Mat> baseImg)
{
	std::cout << "SQUARE X1L = " << X1L << " , disparPX = " << disparPX_ << " , Y1L = " << Y1L << " , order = " << order << " , shapedistance = " << shapedistance << " X2L = " <<X2L<<" , Y2L = " << Y2L <<std::endl;
	std::cout << "SQUARE X1R = " << X1R << " , disparPX = " << disparPX_ << " , Y1R = " << Y1R << " , order = " << order << " , shapedistance = " << shapedistance << " X2R = " << X2R << " , Y2R = " << Y2R << std::endl;
	rectangle(*baseImg, Point((X1L - disparPX_), (Y1L+(int(order) * shapedistance))), Point(X2L - disparPX_, (Y2L + (int(order) * shapedistance))), Scalar(0), FILLED);
	rectangle(*baseImg, Point((X1R + disparPX_), (Y1R + (int(order) * shapedistance))), Point(X2R + disparPX_, (Y2R + (int(order) * shapedistance))), Scalar(0), FILLED);
}

int Square::GetOrder()
{
	return int(order);
}