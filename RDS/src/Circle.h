#ifndef Circle_h
#define Circle_h

#include "Shape.h"
#include <iostream>


class Circle : public Shape
{
public:

	Circle(const float& DisplaySize, const int& DisplayWidth, const int& DisplayHeight, const float& JuleszPixSize,
		const float& ShapeWidth, const float& ShapeHeight, const float& TotalhorizontalMM, const float& TotalVerticalMM,
		float CircleDisparMM, int orderCircle);
	void calcAllParams();
	int GetDisparPX();
	int GetOrder();
	void Draw(std::shared_ptr<cv::Mat> baseImg);

private:
	int centreL_X, centreL_Y, centreR_X, centreR_Y;
	int	radius_;

	enum  position { top = -1, center = 0, bottom = 1 };
	position order;

	int disparPX_;
	float disparMM_;

	void calcDisparPX();
	void calcCentre();
};
#endif