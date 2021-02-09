#ifndef Triangle_h
#define Triangle_h

#include "Shape.h"
#include <iostream>

using namespace cv;


class Triangle : public Shape
{
public:
	Triangle(const float& DisplaySize, const int& DisplayWidth, const int& DisplayHeight, const int& JuleszPixSize,
		const int& ShapeWidth, const int& ShapeHeight, const int& TotalhorizontalMM, const int& TotalVerticalMM,		
		int TriangleDisparMM, int orderTriangle);

	void calcAllParams();
	int GetDisparPX();
	int GetOrder();
	void Draw(std::shared_ptr<cv::Mat> baseImg);

private:
	void calcDisparPX();
	void calcRookPoints(); //calculates the 3 points of the triangle


	int disparPX_;
	float disparMM_;
	cv::Point rook_pointsL[1][3];
	cv::Point rook_pointsR[1][3];
	
	const cv::Point* ppt[1] = { rook_pointsL[0] };
	const cv::Point* ppt2[1] = { rook_pointsR[0] };
	
	enum  position { top = -1, center = 0, bottom = 1 };
	position order;
};

#endif 