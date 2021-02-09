#include "Circle.h"

using namespace cv;

Circle::Circle(const float& DisplaySize, const int& DisplayWidth, const int& DisplayHeight, const float& JuleszPixSize,
	const float& ShapeWidth, const float& ShapeHeight, const float& TotalhorizontalMM, const float& TotalVerticalMM,
	float CircleDisparMM, int orderCircle) : disparMM_(CircleDisparMM)
{

	displaysizeInch_ = DisplaySize;
	displaywidth_ = DisplayWidth;
	displayheight_ = DisplayHeight;
	juleszpixsizeMM_ = JuleszPixSize;
	shapewidthMM = ShapeWidth;
	shapeheightMM = ShapeHeight;
	hordisparMM = TotalhorizontalMM;
	verdisparMM = TotalVerticalMM;
	order = Circle::position(orderCircle);
};

void Circle::calcAllParams()
{
	calcBaseParams();
	calcShapeParams();
	calcDisparPX();
	calcCentre();
}

void Circle::calcDisparPX()
{
	std::cout << "juleszpixsizePX in circle = " << juleszpixsizePX_ << std::endl;
	std::cout << "pixsize in circle = " << pixelsize_ << std::endl;
	disparPX_ = (int(trunc((disparMM_/10) / pixelsize_) / 2) - (int(trunc((disparMM_/10) / pixelsize_) / 2)) % juleszpixsizePX_);
	
}

void Circle::calcCentre()
{
	radius_= shapewidthPX;
	centreL_X = X1L + radius_- disparPX_;
	centreL_Y = Y1L + radius_ + (order * shapedistance);
	centreR_X = X1R + radius_+ disparPX_;
	centreR_Y = Y1R + radius_ + (order * shapedistance);
}

int Circle::GetDisparPX()
{
	return disparPX_;
}

int Circle::GetOrder()
{
	return int(order);
}


void Circle::Draw(std::shared_ptr<cv::Mat> baseImg)
{
	std::cout << "CIRCLE X1L = " << X1L << " , disparPX = " << disparPX_ << " , Y1L = " << Y1L << " , order = " << order << " , shapedistance = " << shapedistance << " X2L = " << X2L << " , Y2L = " << Y2L << std::endl;
	std::cout << "CIRCLE X1R = " << X1R << " , disparPX = " << disparPX_ << " , Y1R = " << Y1R << " , order = " << order << " , shapedistance = " << shapedistance << " X2R = " << X2R << " , Y2R = " << Y2R << std::endl;
	circle(*baseImg, Point(centreL_X, centreL_Y), radius_, Scalar(0), FILLED); //image name, centre point (x,y), radius, color (bgr), line thickness in int, or FILLED
	circle(*baseImg, Point(centreR_X, centreR_Y), radius_, Scalar(0), FILLED);
}