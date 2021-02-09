#include "RDS.h"
#include "BaseImage.h"
using namespace cv;

RDS::RDS(std::unique_ptr<Triangle> tri, std::unique_ptr<Circle> crcl, std::unique_ptr<Square> sqr, int BackgroundColorIndex, int ShapeColorIndex, int imageType) :
  triangle(std::move(tri)), circle(std::move(crcl)), square(std::move(sqr)), backgroundcolor_(TColor(BackgroundColorIndex)), shapecolor_(TColor(ShapeColorIndex))
{
    stimulustype_ = RDS::StimulusType(imageType);
    shapebgr_ = TColorToBGR(shapecolor_);
    backgroundbgr_ = TColorToBGR(backgroundcolor_);
    RDSImg_= std::make_shared <cv::Mat>((square->GetDisplayHeight()), (square->GetDisplayWidth()), CV_8UC3, Scalar(backgroundbgr_.b, backgroundbgr_.g, backgroundbgr_.r));
}

RDS::BGR RDS::TColorToBGR(RDS::TColor colorindex)
{
	RDS::BGR bgr;
    if (colorindex == 0) { bgr.b = 0; bgr.g = 0; bgr.r = 0; bgr.clName = "Black"; 
    }
	else		//clBlack
		if (colorindex == 9) { bgr.b = 0; bgr.g = 0; bgr.r = 255; bgr.clName = "Red";
    }
		else	//clRed
			if (colorindex == 10) { bgr.b = 0; bgr.g = 255; bgr.r = 0; bgr.clName = "Lime";
    }
			else	//clLime
				if (colorindex == 11) { bgr.b = 0; bgr.g = 255; bgr.r = 255; bgr.clName = "Yellow";
    }
				else	//clYellow
					if (colorindex == 12) { bgr.b = 255; bgr.g = 0; bgr.r = 0; bgr.clName = "Blue";
    }
					else	//clBlue
						if (colorindex == 13) { bgr.b = 255; bgr.g = 0; bgr.r = 255; bgr.clName = "Fuchsia";
    }
						else	//clFuchsia
							if (colorindex == 14) { bgr.b = 255; bgr.g = 255; bgr.r = 0; bgr.clName = "Aqua";
    }
							else	//clAqua
								if (colorindex == 15) { bgr.b = 255; bgr.g = 255; bgr.r = 255; bgr.clName = "White";
    };	//clWhite
	return bgr;
}

void RDS::DrawRDSBackGround()
{
   //here square, triangle and circle have the same JuleszPixsize, Shapewidth, etc (calculated from variables in the base classes), so it is enough to call from one of them. Let it be square now.
    int JuleszPixSize = square->GetJuleszPixSizePX_();
	int ShapeWidth = square->GetShapeWidthPX();
    int ShapeHeight = square->GetShapeHeightPX();
    int ImHeight = square->GetDisplayHeight();
    int x;

    int BgrXdb = (roundf((4* ShapeWidth) / ( JuleszPixSize)));
    int BgrYdb = (roundf((ImHeight) / ( JuleszPixSize)));

    int pair;

    for (int pair = 0; pair < 2; pair++)
    {
        srand(123);
        int y = 0;
        for (int j = 0; j < BgrYdb; j++)
        {
            if (pair == 0) 
            { x=(square->X1L) - ShapeWidth;}  
            else
                if (pair == 1) 
                { x = (square->X1R) - ShapeWidth; } 

            for (int i = 0; i < BgrXdb; i++)
            {
               
                int rnd = rand() % 100;
                {
                    if (rnd < 50) {
                   
                        rectangle(*RDSImg_, Point((x), (y)), Point((x + JuleszPixSize), (y + JuleszPixSize)), Scalar(int(shapebgr_.b), int(shapebgr_.g), int(shapebgr_.r)), FILLED);
                    }
                    else
                        if (rnd >= 50) {
                            rectangle(*RDSImg_, Point((x), (y)), Point((x + JuleszPixSize), (y + JuleszPixSize)), Scalar(int(backgroundbgr_.b), int(backgroundbgr_.g), int(backgroundbgr_.r)), FILLED); 
                        }
                }
                x += JuleszPixSize;  
            }
            y +=  JuleszPixSize;
        }
    }
 
}

void RDS::DrawRDSShapePair(std::shared_ptr<cv::Mat> baseImg, bool no_cue)
{
    //stereo shape pair -----------------
    int ShapeWidth = square->GetShapeWidthPX();
    int ShapeHeight = square->GetShapeHeightPX();
    int JuleszPixSize = square->GetJuleszPixSizePX_();
    int x;
    int PX; 
    int SquareDisparPX = square->GetDisparPX();
    int CircleDisparPX = circle->GetDisparPX();
    int TriangleDisparPX = triangle->GetDisparPX();

    int ShXdb = (roundf((4*ShapeWidth) / (JuleszPixSize)));
    int ShYdb = (roundf(2*ShapeWidth / ( JuleszPixSize)));

    //square pair --------------------------
    for (int pair = 0; pair < 2; pair++)
    {
        srand(12);
        int y = square->Y1L + (square->GetOrder()* square->GetShapeDistance());

        try
        {
            if ((y < 0) || (y> square->GetDisplayHeight() ))
                throw std::runtime_error("ERROR: y coordinate is out of display height range in void RDS::DrawRDSShapePair!");
        }
        catch (std::runtime_error e)
        {
            std::cout << e.what() << std::endl;
            exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
        }



        for (int j = 0; j < ShYdb; j++)
        {
            if (pair == 0) { x = square->X1L - SquareDisparPX; }
            else
                if (pair == 1) { x = square->X1R + SquareDisparPX; }

            for (int i = 0; i < ShXdb; i++)
            {
                PX = baseImg->at<uint8_t>(y, x);
                int rnd = rand() % 100;
                if (PX == 0)
                {
                    if (rnd < 50) {
                        if (no_cue == 0)
                        {
                            cv::rectangle(*RDSImg_, Point((x), (y)), Point((x+JuleszPixSize), (y + JuleszPixSize )), Scalar(255, 0, 0), FILLED);
                        }
                       else
                            if (no_cue==1)
                            {
                                cv::rectangle(*RDSImg_, Point((x ), (y)), Point((x + JuleszPixSize), (y + JuleszPixSize )), Scalar(backgroundbgr_.b, backgroundbgr_.g, backgroundbgr_.r), FILLED);
                            }
                    }
                    else
                        if (rnd >= 50) {
                            rectangle(*RDSImg_, Point((x ), (y)), Point((x + JuleszPixSize), (y + JuleszPixSize)), Scalar(shapebgr_.b, shapebgr_.g, shapebgr_.r), FILLED);
                        }
                }
                x += JuleszPixSize;  
            }
            y +=  JuleszPixSize;
        }
    }
    
    //circle pair ---------------------------------

    for (int pair = 0; pair < 2; pair++)
    {
        srand(12);
        int y = circle->Y1L + (circle->GetOrder() * circle->GetShapeDistance());

        try
        {
            if ((y < 0) || (y > square->GetDisplayHeight()))
                throw std::runtime_error("ERROR: y coordinate is out of display height range in void RDS::DrawRDSShapePair!");
        }
        catch (std::runtime_error e)
        {
            std::cout << e.what() << std::endl;
            exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
        }

        for (int j = 0; j < ShYdb; j++)
        {
            if (pair == 0) { x = circle->X1L - CircleDisparPX; }
            else
                if (pair == 1) { x = circle->X1R + CircleDisparPX; }

            for (int i = 0; i < ShXdb; i++)
            {
                PX = baseImg->at<uint8_t>(y, x);
                int rnd = rand() % 100;
                if (PX == 0)
                {
                    if (rnd < 50) {
                        if (no_cue == 0)
                        {
                            cv::rectangle(*RDSImg_, Point((x ), (y)), Point((x + JuleszPixSize), (y + JuleszPixSize )), Scalar(255, 0, 0), FILLED);
                        }
                        else
                            if (no_cue == 1)
                            {
                                cv::rectangle(*RDSImg_, Point((x ), (y)), Point((x+ JuleszPixSize), (y + JuleszPixSize )), Scalar(backgroundbgr_.b, backgroundbgr_.g, backgroundbgr_.r), FILLED);
                            }
                    }
                    else
                        if (rnd >= 50)
                        {
                            rectangle(*RDSImg_, Point((x ), (y)), Point((x+ JuleszPixSize), (y + JuleszPixSize)), Scalar(shapebgr_.b, shapebgr_.g, shapebgr_.r), FILLED);
                        }
                }
                x += JuleszPixSize;
            }
            y +=  JuleszPixSize;
        }
    }

    // triangle pair ----------------------------------
    for (int pair = 0; pair < 2; pair++)
    {
        srand(12);
        int y = triangle->Y1L + (triangle->GetOrder() * triangle->GetShapeDistance());

        try
        {
            if ((y < 0) || (y > square->GetDisplayHeight()))
                throw std::runtime_error("ERROR: y coordinate is out of display height range in void RDS::DrawRDSShapePair!");
        }
        catch (std::runtime_error e)
        {
            std::cout << e.what() << std::endl;
            exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
        }


        for (int j = 0; j < ShYdb; j++)
        {
            if (pair == 0) { x = triangle->X1L - TriangleDisparPX; }
            else
                if (pair == 1) { x = triangle->X1R + TriangleDisparPX; }

            for (int i = 0; i < ShXdb; i++)
            {
                PX = baseImg->at<uint8_t>(y, x);
                int rnd = rand() % 100;
                if (PX == 0)
                {
                    if (rnd < 50) {
                        
                        if (no_cue == 0)
                        {
                            cv::rectangle(*RDSImg_, Point((x ), (y)), Point((x + JuleszPixSize), (y + JuleszPixSize )), Scalar(255, 0, 0), FILLED);
                        }
                        else
                            if (no_cue == 1)
                            {
                                cv::rectangle(*RDSImg_, Point((x ), (y)), Point((x + JuleszPixSize), (y + JuleszPixSize )), Scalar(backgroundbgr_.b, backgroundbgr_.g, backgroundbgr_.r), FILLED);
                            }
                    }
                    else
                        if (rnd >= 50) {
                            
                            rectangle(*RDSImg_, Point((x), (y)), Point((x + JuleszPixSize), (y + JuleszPixSize)), Scalar(shapebgr_.b, shapebgr_.g, shapebgr_.r), FILLED);
                        }
                }
                x +=  JuleszPixSize;  
            }
            y += JuleszPixSize;
        }
    }
   
}


void RDS::DrawSimpleShapePair(std::shared_ptr<cv::Mat> baseImg)
{
    int x;
    int PX;
    int ShapeWidth = square->GetShapeWidthPX();
    for (int pair = 0; pair < 2; pair++)
    {
        
        int y1 = 0;  
        int y2 = square->GetDisplayHeight();  
        for (int j = 0; j < y2; j++)
        {
            if (pair == 0) { x = square->X1L - roundf(ShapeWidth /3); }
            else
                if (pair == 1) { x = square->X1R - roundf(ShapeWidth / 3); }

            for (int i = 0; i < 4*ShapeWidth; i++)
            {
                PX = baseImg->at<uint8_t>(y1, x);
                if (PX == 0)
                {
                    RDSImg_->at<cv::Vec3b>(y1, x)[0] = shapebgr_.b;
                    RDSImg_->at<cv::Vec3b>(y1, x)[1] = shapebgr_.g;
                    RDSImg_->at<cv::Vec3b>(y1, x)[2] = shapebgr_.r;
                }
                x += 1;
            }
            y1 += 1;
        }
    }

}

void RDS::Draw(std::shared_ptr<cv::Mat> baseImg)
{
    if (stimulustype_ == 0)
    {
        DrawSimpleShapePair(baseImg);
    }
    else
        if (stimulustype_ == 1)
        {
            DrawRDSBackGround();
            DrawSimpleShapePair(baseImg);
        }
        else
            if (stimulustype_ == 2)
            {
                DrawRDSBackGround();
                DrawRDSShapePair(baseImg, 0);
            }
            else
                if (stimulustype_ == 3)
                {
                    DrawRDSBackGround();
                    DrawRDSShapePair(baseImg, 1);
                }
}
    

std::string RDS::GenerateSolutionKey(std::shared_ptr<cv::Mat> baseImg)
{
     std::string DepthOrder;

    int depthCircle = circle->GetDisparPX();
    int depthTriangle = triangle->GetDisparPX();
    int depthSquare = square->GetDisparPX();

    if ((depthSquare < depthCircle) && (depthCircle < depthTriangle))  DepthOrder  = "Square, Circle, Triangle";
    if ((depthSquare < depthTriangle) && (depthTriangle < depthCircle))  DepthOrder  = "Square, Triangle, Circle";
    if ((depthTriangle < depthCircle) && (depthCircle < depthSquare))  DepthOrder  = "Triangle, Circle, Square";
    if ((depthTriangle < depthSquare) && (depthSquare < depthCircle))  DepthOrder  = "Triangle, Square, Circle";
    if ((depthCircle < depthTriangle) && (depthTriangle < depthSquare))  DepthOrder  = "Circle, Triangle, Square";
    if ((depthCircle < depthSquare) && (depthSquare < depthTriangle))  DepthOrder  = "Circle, Square, Triangle";

    return backgroundbgr_.clName + " - " + shapebgr_.clName + " = " + DepthOrder;

      

}
