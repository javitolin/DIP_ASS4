//#include <cv.h>   		// open cv general include file
//#include <highgui.h>	// open cv GUI include file
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>		// standard C++ I/O
#include <string.h>

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace std;

string usageMsg = "Usage is FilterImage input_image [-L|-H] [-B D0 n|-G sigma]  [-o filtered_image, -f fourier_image]\n";
string input_image = "", filtered_image = "", fourier_image = "";
bool isHigh = false;
bool isButter = false;
int D0 = 1;
int n = 1;
int sigma = 1;

void errVars(){
	std::cout << "Not enough or invalid arguments, please try again.\n";
	std::cin.get();
}

void printVars(){
	std::cout << "input_image: "    << input_image    << endl;
	std::cout << "filtered_image: " << filtered_image << endl;
	std::cout << "fourier_image: "  << fourier_image  << endl;
	std::cout << "isHigh: "         << isHigh         << endl;
	std::cout << "isButter: "       << isButter       << endl;
	std::cout << "D0: "             << D0             << endl;
	std::cout << "n: "              << n              << endl;
	std::cout << "sigma: "          << sigma          << endl;
}

void create_butterworth_highpass_filter(CvMat* dft_Filter, int D, int n)
{
	CvMat* single = cvCreateMat(dft_Filter->rows, dft_Filter->cols, CV_64FC1 );

	CvPoint centre = cvPoint(dft_Filter->rows / 2, dft_Filter->cols / 2);
	double radius;

	// based on the forumla in the IP notes (p. 124 of 2009/10 version)
	// see also HIPR2 on-line

	for(int i = 0; i < dft_Filter->rows; i++)
	{
		for(int j = 0; j < dft_Filter->cols; j++)
		{
			radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
			CV_MAT_ELEM(*single, double, i, j) =
						( 1 / (1 + pow((double) (D /  radius), (double) (2 * n))));
		}
	}

	cvMerge(single, single, NULL, NULL, dft_Filter);

	cvReleaseMat(&single);
}

void create_butterworth_lowpass_filter(Mat &dft_Filter, int D, int n, int W)
{
    Mat tmp = Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

    Point centre = Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
    double radius;

    for (int i = 0; i < dft_Filter.rows; i++)
    {
        for (int j = 0; j < dft_Filter.cols; j++)
        {
            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));

            // Butterworth low pass:
            tmp.at<float>(i, j) = (float)
                                  ( 1 / (1 + pow((double) (radius * W) / ( pow((double)radius, 2) - D * D ), (double) (2 * n))));
        }
    }

    Mat toMerge[] = {tmp, tmp};
    merge(toMerge, 2, dft_Filter);
}

// When passing char arrays as parameters they must be pointers
int main(int argc, char* argv[]) {

    /*  Get User Input */
    if (argc < 5) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        std::cout << usageMsg; // Inform the user of how to use the program
        std::cin.get();
        exit(0);
    } else { // if we got enough parameters...
	input_image = argv[1];
        for (int i = 2; i < argc; i++) { /* We will iterate over argv[] to get the parameters stored inside.
                                          * Note that we're starting on 2 because we don't need to know the 
                                          * path of the program, which is stored in argv[0] and the input_image which is always stored in argv[1] */

                if (strcmp(argv[i],"-o") == 0) {
		    if (i + 1 >= argc){
			errVars(); exit(0);}
                    filtered_image = argv[i + 1];
                } else if (strcmp(argv[i],"-f") == 0){
		    if (i + 1 >= argc){
			errVars(); exit(0);}
                    fourier_image = argv[i + 1];
                } else if (strcmp(argv[i],"-H") == 0){
                    isHigh = true;
                } else if (strcmp(argv[i],"-L") == 0){
                    isHigh = false;
                } else if (strcmp(argv[i],"-B") == 0){
		    if (i + 2 >= argc){
			errVars(); exit(0);}
                    isButter = true;
		    D0 = atoi(argv[i + 1]);
		    n  = atoi(argv[i + 2]);
                } else if (strcmp(argv[i],"-G") == 0){
                    isButter = false;
		    sigma = atoi(argv[i + 1]);
                }
        }

        Mat image = imread(input_image);
        imshow("New Window", image);
        //Gaussian getGaussianKernel(1,sigma);
    }
    /*  Completed User Input */
	


     std::cin.get();
     return 0;
}

