#ifndef Square_h
#define Square_h

#include "Shape.h"
#include <iostream>
#include <memory>

class Square : public Shape
{
public:

	Square(const float& DisplaySize, const int& DisplayWidth, const int& DisplayHeight, const float& JuleszPixSize,
		const float& ShapeWidth, const float& ShapeHeight, const float& TotalhorizontalMM, const float& TotalVerticalMM,
		float SquareDisparMM, int orderSquare);

	int GetDisparPX();
	int GetOrder();  //returns the top, center or bottom position of the square shape
	void Draw(std::shared_ptr<cv::Mat> baseImg); //overload of the virtual function in Shape.h
	void calcAllParams();
private:
	void calcDisparPX();  //calculates the disparity of the square pair from 1/10 mm to pixels
	int disparPX_;  //calculated from disparMM
	float disparMM_; //disparity of the square shape pair given in 1/10 mm in the input file

	enum  position { top = -1, center = 0, bottom = 1 };
	position order;
};
#endif