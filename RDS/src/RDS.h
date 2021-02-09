#ifndef RDS_h
#define RDS_h

#include "Shape.h"
#include "Circle.h"
#include "Square.h"
#include "Triangle.h"
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <fstream>

class RDS
{
public:
	RDS(std::unique_ptr<Triangle> tri, std::unique_ptr<Circle> crcl, std::unique_ptr<Square> sqr, int BackgroundColorIndex, int ShapeColorIndex, int imageType);
	void DrawRDSBackGround();  //draws the random dot background
	void DrawRDSShapePair(std::shared_ptr<cv::Mat> baseImg, bool no_cue); //draws the random dot shape pair by accessing the base image pixel by pixel  to get the shapes
	void DrawSimpleShapePair(std::shared_ptr<cv::Mat> baseImg); //draws a filled shape pair, copying the shapes from the base image pixel by pixel, in the wished brg color
	void Draw(std::shared_ptr<cv::Mat> baseImg);
	std::shared_ptr<cv::Mat> RDSImg_;  //the heap is used because it is a high resolution image. Shared pinter is used because more than one functions need to use it, and they need access also multiple times
	std::string GenerateSolutionKey(std::shared_ptr<cv::Mat> baseImg); //generates a textfile in which the right 3D order of each images is written to serve for the experimenter as a hint for the expected response for each image
private:
	
	std::unique_ptr<Triangle> triangle; //heap is used because they are needed through the whole process. unique pointer is used because they can be moved, no more access is needed from main, from where they are moved here.
	std::unique_ptr<Circle> circle;
	std::unique_ptr<Square> square;

	//color indexes as stored in the ini file
	enum  TColor {
		clBlack=0,  //color indexes are from my original program written in Delphi - indexes in the ColorListBox, a GUI element there. Coded here so in order to be able to use the opd input files
		clRed=9,
		clLime=10,
		clYellow=11,
		clBlue=12,
		clFuchsia=13,
		clAqua=14,
		clWhite=15
	};

	TColor shapecolor_;
	TColor backgroundcolor_;

	enum  StimulusType
	{
		simple=0,			//filled background, filled shape
		RDSFilledShape=1,   //random dot background, filled shape
		RDSDiffColors=2,	//random dot background and shape, in different colors
		RDSclassic=3		//random dot background and shape, same colors: the only cue is the disparity
	};
	
	StimulusType stimulustype_;


	//rgb color values, as passed into the cv function
	struct BGR
	{
		int b, g, r;
		std::string clName;
	};

	BGR shapebgr_;
	BGR backgroundbgr_;

	//convert color indexes to brg values
	BGR TColorToBGR(TColor);
	

};
#endif