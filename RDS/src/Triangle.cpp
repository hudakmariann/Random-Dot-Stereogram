#include "Triangle.h"
#include <iostream>


Triangle::Triangle(const float& DisplaySize, const int& DisplayWidth, const int& DisplayHeight, const int& JuleszPixSize,
	const int& ShapeWidth, const int& ShapeHeight, const int& TotalhorizontalMM, const int& TotalVerticalMM,	
	int TriangleDisparMM, int orderTriangle) : disparMM_(TriangleDisparMM)
{
	displaysizeInch_ = DisplaySize;
	displaywidth_ = DisplayWidth;
	displayheight_ = DisplayHeight;
	juleszpixsizeMM_ = JuleszPixSize;
	shapewidthMM = ShapeWidth;
	shapeheightMM = ShapeHeight;
	hordisparMM = TotalhorizontalMM;
	verdisparMM = TotalVerticalMM;
	order = Triangle::position(orderTriangle);


	

}

void Triangle::calcAllParams()
{
	calcBaseParams();
	calcShapeParams();
	calcDisparPX();
	calcRookPoints();
}

void Triangle::calcDisparPX()
{
	
	std::cout << "juleszpixsizePX in triangle = " << juleszpixsizePX_ << std::endl;
	std::cout << "pixsize in triangle = " << pixelsize_ << std::endl;
	disparPX_ = (int(trunc((disparMM_/10) / pixelsize_) / 2) - (int(trunc((disparMM_/10) / pixelsize_) / 2)) % juleszpixsizePX_);
}

int Triangle::GetDisparPX()
{
	return disparPX_;
}

void Triangle::calcRookPoints()
{
	//http://opencvexamples.blogspot.com/2013/10/basic-drawing-examples.html
//this example works both here and in Visual Studio 2019 on Windows
	rook_pointsL[0][0] = Point((X1L - disparPX_), Y2L + (order * shapedistance));
	rook_pointsL[0][1] = Point(((X1L - disparPX_ + round(shapewidthPX))), (Y1L + (order * shapedistance)));
	rook_pointsL[0][2] = Point((X2L - disparPX_), (Y2L + (order * shapedistance)));

	rook_pointsR[0][0] = Point((X1R + disparPX_), (Y2R + (order * shapedistance)));
	rook_pointsR[0][1] = Point(((X1R + disparPX_ +round(shapewidthPX ))), (Y1R + (order * shapedistance)));
	rook_pointsR[0][2] = Point((X2R + disparPX_), (Y2R + (order * shapedistance)));
}

void Triangle::Draw(std::shared_ptr<cv::Mat> baseImg)
{
	calcRookPoints();
	int npt[] = { 3 };
	int npt2[] = { 3 };

	std::cout << "TRIANGLE X1L = " << X1L << " , disparPX = " << disparPX_ << " , Y1L = " << Y1L << " , order = " << order << " , shapedistance = " << shapedistance << " X2L = " << X2L << " , Y2L = " << Y2L << std::endl;
	std::cout << "TRIANGLE X1R = " << X1R << " , disparPX = " << disparPX_ << " , Y1R = " << Y1R << " , order = " << order << " , shapedistance = " << shapedistance << " X2R = " << X2R << " , Y2R = " << Y2R << std::endl;
	
	fillPoly(*baseImg, ppt, npt, 1, Scalar(0), 8);
	fillPoly(*baseImg, ppt2, npt2, 1, Scalar(0), 8);
}

int Triangle::GetOrder()
{
	return int(order);
}





