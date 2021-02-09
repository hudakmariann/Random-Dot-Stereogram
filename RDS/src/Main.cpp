#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


//https://learnopencv.com/cvui-gui-lib-built-on-top-of-opencv-drawing-primitives/
//https://dovyski.github.io/cvui/usage/
#include <opencv2/opencv.hpp>
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#define WINDOW_NAME "Random Dot Stereogram"


#include <iostream>
#include <vector>
#include <stdint.h>
#include <memory>

#include "BaseImage.h"
#include "RDS.h"
#include "Shape.h"
#include "Circle.h"
#include "Square.h"
#include "Triangle.h"

#include <thread>
#include <fstream>
#include <future>

#include <string>

using namespace cv;
using namespace std;

#include <sys/stat.h> // to check if file / path exists - https://www.systutorials.com/how-to-test-a-file-or-directory-exists-in-c/


//this struct is for the purpose of passing the images between the Main and other functions for showing and saving them from the GUI.
struct Images {
    shared_ptr<cv::Mat>BaseImg;
    shared_ptr<cv::Mat>RDSImg;
};

string initClasses(std::vector<std::pair<string, float>> &&Params, Images& Img, string *FN)
{
     string SolutionKey; //this is for the solution textfile of the image series

    //checking whether the Params come in the expected order (to check whether the file contain valid data or the file is not damaged)
    vector<string>ParamNames{ "DisplaySize" ,"DisplayWidth","DisplayHeight","JuleszPixSizeMM","ShapeWidthMM","ShapeHeightMM",
    "TotalHorizontalMM" ,"TotalVerticalMM" , "SquareDisparMM" , "TriangleDisparMM" , "CircleDisparMM" , 
        "orderSquare" , "orderTriangle", "orderCircle",  "ShapeColorIndex","BackgroundColorIndex", "imageType"};
   
        std::future<void> ExpHandling0 = std::async(
            [&Params, &ParamNames]()
            {
                try
                {
                    int i = 0;
                    std::cout <<Params.size()<< " = " << ParamNames.size() << std::endl;
                    for (auto P : Params)
                    {
                        std::cout << P.first << "=" <<P.second<< std::endl;
                        if ((Params.size() != ParamNames.size())||(P.first != ParamNames[i]))
                            throw std::runtime_error("ERROR: bad parameter list in input file");
                        std::cout <<P.first << " = " << ParamNames[i]<< std::endl;
                        i++;
                    }
                }
                catch (std::runtime_error e)
                {
                    std::cout << e.what() << std::endl;
                    exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
                }
            });
        ExpHandling0.get();
       
//Reason for defining named local variabes instead of using the vector elements is that the code is easier to read this way and there is less risk of error when the class constructors are initialized by these below.
        //BaseImage Params    
        float DisplaySize = Params[0].second;
        int DisplayWidth = int(Params[1].second);
        int  DisplayHeight = int(Params[2].second);
        float JuleszPixSizeMM = (Params[3].second);
        // Shape params
        float ShapeWidthMM = (Params[4].second);
        float  ShapeHeightMM = (Params[4].second); //ShapeHeight turned out to be a superfluous parameter, it shoud be equal to shapewith - but now it is too risky to remove it from everywhere where it was referred to. Thus it is initialized to the ShapeWidth value.
        float  TotalhorizontalMM = (Params[6].second);
        float TotalVerticalMM = (Params[7].second);
        float SquareDisparMM = (Params[8].second);
        float TriangleDisparMM = (Params[9].second);
        float CircleDisparMM = (Params[10].second);
        int  orderSquare = int(Params[11].second);
        int  orderTriangle = int(Params[12].second);
        int  orderCircle = int(Params[13].second);
        //RDS Params
        int  ShapeColorIndex = int(Params[14].second);
        int  BackgroundColorIndex = int(Params[15].second);
        int  imageType = int(Params[16].second);

    //checking display size
        //https://www.bfilipek.com/2020/05/lambdas-async.html#lambdas-with-stdasync
        std::future<void> ExpHandling1 = std::async(
            [&DisplaySize, &DisplayWidth, &DisplayHeight]()
            {
                try
                {
                    if ((DisplayWidth < 640) || (DisplayHeight < 480) || (DisplaySize < 4))
                        throw std::runtime_error("ERROR: invalid display size!");
                }
                catch (std::runtime_error e)
                {
                    std::cout << e.what() << std::endl;
                    exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
                }
            });

      //checking shape positions
        std::future<void> ExpHandling2 = std::async(
            [&orderSquare, &orderTriangle, &orderCircle]()
            {
                try
                {
                    if ((orderSquare < -1) || (orderSquare > 1) || (orderCircle < -1) || (orderCircle > 1) || (orderTriangle < -1) || (orderTriangle > 1))
                        throw std::runtime_error("ERROR: invalid shape positions in input file!");
                }
                catch (std::runtime_error e)
                {
                    std::cout << e.what() << std::endl;
                    exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
                }
            });

        //checking accepted color indexes and image type
        //color indexes here correspond to the indexes in the Delphi GUI component for selecting the wished color - it was coded so in my original Delphi program, and I coded it so here as well, so that the old input csv files can be re-used.
        std::future<void> ExpHandling3 = std::async(
            [&ShapeColorIndex, &BackgroundColorIndex, &imageType]()
            {
                try
                {
                    if (((ShapeColorIndex < 9)&&(ShapeColorIndex!=0)) || (ShapeColorIndex > 15) || ((BackgroundColorIndex < 9)&&(BackgroundColorIndex!=0)) || (BackgroundColorIndex > 15))
                    throw std::runtime_error("ERROR: invalid color index in input file!");   
                    if ((imageType < 0) || (imageType > 3)) 
                        throw std::runtime_error("ERROR: invalid image type index in input file!");
                }
                catch (std::runtime_error e)
                {
                    std::cout << e.what() << std::endl;
                    exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
                }
            });
  
        ExpHandling1.get();
        ExpHandling2.get();
        ExpHandling3.get();

    //the reason for using the heap here below is that this is a high resolution image which involves a large amount of data, and it is needed throughout the drawing of the final image.
    //shared_ptr is used because the image has to be accessed by multiple functions , from more than one classes and threads.
    //in accordance with the principle of encapsulation, I'm not sure whether OpenCV Mat uses the heap or not, and whether it uses smart pointers internally or not, so I'm using the shared pointer around it, just to make sure to use the heap memory correctly.
      
        std::shared_ptr<cv::Mat> baseImg(new cv::Mat(DisplayHeight, DisplayWidth, CV_8UC1, cv::Scalar(255)));
       //(alternative definition of a shared_ptr: - for myself for future reference
       // baseImg = std::make_shared <cv::Mat>(DisplayHeight, DisplayWidth, CV_8UC1, cv::Scalar(255)); 


    //the reason for using the heap here is that the member values of these objects below are needed throughout the drawing of the final image, and this is a larger amount of data.
    //the reason for using unique pointer is that these are needed only elsewhere, not here, so they can be moved.
        std::unique_ptr<Square> square(new Square(DisplaySize, DisplayWidth, DisplayHeight, JuleszPixSizeMM,
             ShapeWidthMM, ShapeHeightMM, TotalhorizontalMM, TotalVerticalMM,
             SquareDisparMM, orderSquare));
        std::unique_ptr<Triangle> triangle(new Triangle (DisplaySize, DisplayWidth, DisplayHeight, JuleszPixSizeMM,
            ShapeWidthMM, ShapeHeightMM, TotalhorizontalMM, TotalVerticalMM,            
            TriangleDisparMM, orderTriangle));
        std::unique_ptr<Circle> circle(new Circle (DisplaySize, DisplayWidth, DisplayHeight, JuleszPixSizeMM,
            ShapeWidthMM, ShapeHeightMM, TotalhorizontalMM, TotalVerticalMM,            
            CircleDisparMM, orderCircle));

        //QUESTION: is there any case, when it is worth implementing the rule of five for a class instead of using a smart pointer?

        std::thread tSquare = std::thread(&Square::Square::calcAllParams, square.get()); //the last argument needs the address of the original pointer embedded in the shared ptr
        std::thread tTriangle = std::thread(&Triangle::Triangle::calcAllParams, triangle.get());
        std::thread tCircle = std::thread(&Circle::Circle::calcAllParams, circle.get());

        tSquare.join();
        tTriangle.join();
        tCircle.join();

        square->Draw(baseImg);
        triangle->Draw(baseImg);
        circle->Draw(baseImg);

     //creating the instance of the final image class:
        RDS rds(std::move(triangle), std::move(circle), std::move(square), ShapeColorIndex, BackgroundColorIndex, imageType);
        
    //the reason for starting this RDS::Draw function in a separate thread is that it involves reading a high resolution image pixel by pixel, and also modifying many pixels there, pixels by pixels. Lots of work.
        std::thread tRDS = std::thread(&RDS::RDS::Draw, &rds, baseImg );
        tRDS.join();
        Img.BaseImg=baseImg;
        Img.RDSImg=rds.RDSImg_;
        SolutionKey = rds.GenerateSolutionKey(baseImg);
        baseImg.reset();
        rds.RDSImg_.reset();
       
        //the images are saved automatically only if the 'Save image series' button is hit. If the user generates a single image, it is saved by hitting a sepatare button, so nullptr is passed here in that case.        
        if (FN != nullptr) {
            imwrite(*FN, *Img.RDSImg);
            std::cout << *FN << " image saved \n";
        }

        return SolutionKey;
}


//// to check if file / path exists - https://www.systutorials.com/how-to-test-a-file-or-directory-exists-in-c/
//this function is called from parseinifile, parse csv file, and it also checks the existance of the output directory
bool PathExist(const std::string& s)
{
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

//reading default parameters from the default ini file. The file can be modifyied and saved later on, and re-read and re-drawn by hitting the corresponding button on the GUI, without closing and restarting the program.
std::vector<std::pair<string, float>> ParseIniFile(bool const &use_default_input)
{
    string inifile;
    if (!use_default_input)
    {
        std::cout << "Please type the input filename in format '../directoryname/filename.ini'  \n";
        std::cout << "To use the default file, please type in './RDS.ini'  \n";
        std::cin >> inifile;
        std::size_t found = (inifile.find(".ini")); //http://www.cplusplus.com/reference/string/string/find/

        //if the user forgets to type in the file extension, the program automatically corrects it
        if (found == std::string::npos)
            inifile += ".ini";

        //if the user types in a non-existing path, then it asks the user to re-type it until he/she types in an existing path
        while (!PathExist(inifile))
        {
            std::cout << "FAILURE: Path or .ini file does not exist. Please type the input filename in format '../directoryname/filename.ini'  \n";
            std::cout << "To use the default file, please type in './RDS.ini'  \n";
            std::cin >> inifile;
        }
        std::cout << inifile << " SUCCESS: Correct path name! \n";
    }
    else
    {
        try
        {
            if (PathExist("./RDS.ini")) 
            {
                inifile = "./RDS.ini";
            }
            else
            {
                throw std::runtime_error("ERROR: Default *.ini file RDS.ini does not exist!");
            }
        }
        catch (std::runtime_error e)
        {
            std::cout << e.what() << std::endl;
            exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
        }
    }
    
    string line;
    string key;
    string oper;
    float value;
    std::pair<string, float> datapair;
    std::vector<std::pair<string, float>> Params;
    std::ifstream filestream(inifile);
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            while (linestream >> key >> oper >> value)
            {
                datapair = std::make_pair(key, value);
                //std::cout << "key = " << datapair.first << " , value = " << datapair.second << std::endl; //for debugging
                Params.push_back(datapair);
            }
        }
    }
    return (std::move(Params));
    //in an earlier version, I tried to return this as a pointer, however, it did not work, I still do not understand, why.
    /*std::vector<std::pair<string, float>>* ParamsPtr = &Params; //pointer to vector needs to be initialized: http://www.cplusplus.com/forum/general/32374/
    return ParamsPtr;*/
}

void  ParseCSVFile(bool const &use_default_input, bool const &use_default_output, bool const &save_solution)
{
    string csvfile;
    string DeviceName;
    string Solution;
    vector<string>SolutionVec;

    if (!use_default_input)
    {
        std::cout << "Please type the input filename in format '../directoryname/filename.csv'  \n";
        std::cout << "To use the default file, please type in './SamsungGalaxyS7.csv'  \n";
        std::cin >> csvfile;
        std::size_t found = (csvfile.find(".csv")); //http://www.cplusplus.com/reference/string/string/find/

        //if the user forgets to type in the file extension, the program automatically corrects it
        if (found == std::string::npos)
            csvfile += ".csv";

        //if the user types in a non-existent path, then it asks the user to re-type it until he/she types in an existing path
        while (!PathExist(csvfile))
        {
            std::cout << "FAILURE: path or CSV file does not exist. Please type the input filename in format '../directoryname/filename.csv'  \n";
            std::cout << "To use the default file, please type in './SamsungGalaxyS7.csv'  \n";
            std::cin >> csvfile;
        }
        std::cout << csvfile << " SUCCESS: Correct CSV file name! \n";
    }
    else
        
        {
        //if, for some reason, the default file gets damaged or deleted, an exception is caught
            try
            {
                if (PathExist("./SamsungGalaxyS7.csv"))
                    csvfile = "./SamsungGalaxyS7.csv";
                else
                    throw std::runtime_error("ERROR: Default csv file SamsungGalaxyS7.csv does not exist!");
            }
            catch (std::runtime_error e)
            {
                std::cout << e.what() << std::endl;
                exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
            }
        }

//the user can provide an output directory other than the default, if the corresponding checkbox is not checked.
    string outputDir;
    if (!use_default_output)
    {
        std::cout << "Please type an EXISTING output directory name where you wish to save the images, in format: '../directoryname  \n";
        std::cout << "To use the default output directory, please type in './outputimages'  \n";
        std::cin >> outputDir;

        while (!PathExist(outputDir))
        {
            std::cout << "FAILURE: Path does not exist. Please type the output directory name where you wish to save the images, in format: '../directoryname  \n";
            std::cout << "To use the default output directory, please type in './outputimages'  \n";
            std::cin >> outputDir;
        }
        std::cout << outputDir << " SUCCESS: Correct output directory name! \n";
    }
    else

    {
        try
        {
            if (PathExist("./outputimages"))
                outputDir = "./outputimages";
            else
                throw std::runtime_error("ERROR: Default output directory './outputimages' does not exist!");
        }
        catch (std::runtime_error e)
        {
            std::cout << e.what() << std::endl;
            exit(EXIT_FAILURE); //https://stackoverflow.com/questions/32257840/properly-terminating-program-using-exceptions
        }
    }

    
    //reading in data from the csv file:
    string line;
    string key;
    string oper;
   
    float value;
    int linecount = 0;
  
    std::pair<string, float> datapair;
    vector<string> ParamNames;
    vector<float> values;
    vector<std::pair<string, float>> Params;
    std::ifstream filestream(csvfile);
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            values.clear();
            Params.clear();
            std::cout << line << std::endl;
            std::replace(line.begin(), line.end(), ';', ' ');
            std::cout << line << std::endl;
            std::istringstream linestream(line);
            if (linecount == 0)
            {
                int paramcount = 0;
                while (linestream >> key)
                {
                    if (paramcount!=0)
                    ParamNames.push_back(key);
                    paramcount++;
                }
            }
            if (linecount>0)
            {
                if ((linestream >> key))  DeviceName = key;
                
                    while (linestream >> value)
                    {
                     values.push_back(value);
                    }
                Images Image;
                string FN;
                if (linecount < 10)
                    FN = outputDir +"/"+DeviceName+"_0" + std::to_string(linecount) + ".bmp";
                    //std::cout << FN << " = FN FILEname  \n";
                if (linecount >= 10)
                     FN = outputDir + "/" + DeviceName + "_" + std::to_string(linecount) + ".bmp";
                   // std::cout << FN << " = FN FILEname  \n";
                int i = 0;
                for (auto PN : ParamNames)
                {
                    datapair = std::make_pair(PN, values[i]);
                    Params.push_back(datapair);
                    i++;
                }
                //if checkbox save_soulution is checked, it saves the right answers to a textfile, to make it easier for the experimenter to check the responses of the subject
                Solution=initClasses(std::move(Params), Image, &FN);
                if (save_solution) SolutionVec.push_back(to_string(linecount)+". "+Solution);

              // QUESTION: if I want to pass this in a thread with the code in the comment below, it does not compile. Why?
              //I wanted to make a vector of threads here, to make it faster, but here it does not even compile as a single thread.
              // QUESTION: would it be worth anyway to put this in a thread? I'm not sure, because initClasses launches other threads anyway. 
              //std::thread CreateImage(&initClasses, Params, Image, &FN);
              // CreateImage.join();
            }
            linecount++;
        }
    }

    //if checkbox save_soulution is checked, it saves the right answers to a textfile, to make it easier for the experimenter to check the responses of the subject
    if (save_solution)
    {
        string SolutionFN = outputDir + "/" + DeviceName + ".txt";
        std::ofstream fout(SolutionFN);
        for (auto SL : SolutionVec)
        {
            fout << SL << endl;
        }
    }
}


int main() 
{
    //create an Images struct to access the shared_ptr-s to the base and RDS images from the main, created by other functions
    Images Image;
    //local variables that can be passed to functions from the GUI elements
    bool use_default_input;
    bool use_default_output;
    bool save_solution;

    //creating a simple GUI:
    //https://learnopencv.com/cvui-gui-lib-built-on-top-of-opencv-drawing-primitives/
    cv::Mat frame = cv::Mat(400, 300, CV_8UC3);       
        // Init a OpenCV window and tell cvui to use it.
        cv::namedWindow(WINDOW_NAME);
        cvui::init(WINDOW_NAME);
        while (true) {
            // Fill the frame with a nice color
            frame = cv::Scalar(50, 50, 50);

            cvui::checkbox(frame, 15, 230, "Use default input (*.ini or *.csv) file", &use_default_input);
            cvui::checkbox(frame, 15, 260, "Use default output dir (./outputimages)", &use_default_output);
            cvui::checkbox(frame, 15, 290, "Save Solution textfile", &save_solution);
            // Show a button at position  (15, 60):
            if (cvui::button(frame, 15, 60, "Create image matrix"))
            {
                //create the instance of the base and the RDS images from the default ini file:
                //the data read from the ini file are moved to the init classes function, along with the instance of Images created here, so that the shared pointers to the images will be accessible from the main function.
                //nullptr is passed to the Filename attribute of InitClasses, since here, when creating the single image, we do not want to save the output image automatically, no file name is necessary.
                //however, a non-empty ptr to the filename is passed when the image series is generated by the ParseCSV function - there filenames are automatically generated and their pointers are passed to Initclasses.
                //for this parameter, I considered it unnecessary to use the heap, because it is not very data-consuming, but it might be more data to copy to pass the string itself, than its address, that"s why I used a raw pointer here.
                initClasses(std::move(ParseIniFile(use_default_input)), Image, nullptr);
            }
            if (cvui::button(frame, 15, 90, "Show base image   "))
            {
                if (Image.BaseImg)
                {
                    //show the base image:
                    cv::imshow("BaseIamge", *Image.BaseImg);
                    waitKey();
                }
                else std::cout << "WARNING: no baseimage exists. \n" << "Hit the 'Create image matrix' button! \n";
            }
            if (cvui::button(frame, 15, 120, "Show RDS image   "))
            {
                if (Image.RDSImg)
                {
                    //show the final RDS image
                    cv::imshow("RDSIamge", *Image.RDSImg);
                    waitKey();
                }
                else std::cout << "WARNING: no RDSimage exists. \n" << "Hit the 'Create image' button! \n";
            }
            if (cvui::button(frame, 15, 150, "Save RDS image   "))
            {
                if (Image.RDSImg)
                {
                    //save the current RDS image
                    cv::imwrite("RDS.bmp", *Image.RDSImg);
                }
                else std::cout << "WARNING: no RDSimage exists. \n" << "Hit the 'Create image' button! \n";
            }

            if (cvui::button(frame, 15, 180, "Save RDS series  "))
            {
                ParseCSVFile(use_default_input, use_default_output, save_solution);
            }

            cvui::update();

            cvui::printf(frame, 15, 10, 0.4, 0xdddddd, "Hit Esc to close this window.");
            cvui::printf(frame, 15, 25, 0.4, 0xdddddd, "Press any key to re-activate buttons.");

            // Update cvui internal stuff
            cvui::update();

            // Show everything on the screen
            cv::imshow(WINDOW_NAME, frame);

            // Check if ESC key was pressed

            if (cv::waitKey(20) == 27) { //27 is the code number of the Esc key
                break;
            }
        }
    
    return 0;
}