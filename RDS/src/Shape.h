#ifndef Shape_h
#define Shape_h

#include <iostream>
#include "BaseImage.h"


class Shape : public BaseImage
{
public:
	
	//These are public, since they need to be accessed by the RDS class instance through the child class instances of Shape
	//they return the private values
	int GetShapeHeightPX();
	int GetShapeDistance();
	int GetShapeWidthPX();

	int X1L; //raw common coordinates of shapes, modified by the order (top, middle or bottom position), and the disparity of each shape
	int X1R;
	int Y1L;
	int Y1R;
	int X2L;
	int X2R;
	int Y2L;
	int Y2R;

private:
//these need not to be accessed from outside one by one, so they are private
	void calcShapeDistance(); //calculates the vertical distance between shapes in pixels
	void calcCoordinates(); //calculates X1L, Y1L, etc...
	void calcShapeSizePX(); //calc shape size in pixels
	void calcHorDisparPX();  //calcs the horizontal distance of the left and right groups of shapes to pixels from the input file where it is given in  mm
	void calcVerDisparPX(); //calc the verical disparity of left and right groups of shapes to pixels from the input file where it is given in  mm
	//the latter is by default 0, but it can be adjusted if needed for eg. a strabismic subject
	
protected:

	//these are protected, since they only need to be accesed by instances of child classes
void calcShapeParams(); //calls are shape param calculations

float hordisparMM;     //horizontal disparity given by the user , same for all shapes
int hordisparPX;     // real horizntal disparity calculated from the user-given value and the real pixel size in pixels  , same for all shapes
float verdisparMM;  //vertical disparity between the left and right groups of shapes inpixels
int verdisparPX;
int shapedistance;


float shapewidthMM; //shape width in the input files in 1/10 mm
int shapewidthPX; //calculated shape width in pixels
float shapeheightMM;
int shapeheightPX;


virtual void Draw() {};  //this function is overloaded by each shape's Draw function

};

#endif


