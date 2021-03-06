#include <iostream>	// for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
/*
#include <opencv/cxcore.h>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv/highgui.h>  // OpenCV window I/O
#include <opencv\ml.h>
*/
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "cvaux.h"

using namespace std;
using namespace cv;

double getPSNR ( const Mat& I1, const Mat& I2);
Scalar getMSSIM( const Mat& I1, const Mat& I2);

void help()
{
    cout
        << "\n--------------------------------------------------------------------------" << endl
        << "This program shows how to read a video file with OpenCV. In addition, it tests the"
        << " similarity of two input videos first with PSNR, and for the frames below a PSNR "  << endl
        << "trigger value, also with MSSIM."<< endl
        << "Usage:"                                                                       << endl
        << "./video-source referenceVideo useCaseTestVideo PSNR_Trigger_Value Wait_Between_Frames " << endl
        << "--------------------------------------------------------------------------"   << endl
        << endl;
}
int main(int argc, char *argv[], char *window_name)
{
    help();
    if (argc != 5)
    {
        cout << "Not enough parameters" << endl;
        return -1;
    }
    stringstream conv;

    const string sourceReference = argv[1],sourceCompareWith = argv[2];
    int psnrTriggerValue, delay;
    conv << argv[3] << endl << argv[4];		  // put in the strings
    conv >> psnrTriggerValue >> delay;// take out the numbers

    char c;
    int frameNum = -1;			// Frame counter

    VideoCapture captRefrnc(sourceReference),
        captUndTst(sourceCompareWith);

    if ( !captRefrnc.isOpened())
    {
        cout  << "Could not open reference " << sourceReference << endl;
        return -1;
    }

    if( !captUndTst.isOpened())
    {
        cout  << "Could not open case test " << sourceCompareWith << endl;
        return -1;
    }

    Size refS = Size((int) captRefrnc.get(CV_CAP_PROP_FRAME_WIDTH),
        (int) captRefrnc.get(CV_CAP_PROP_FRAME_HEIGHT)),
        uTSi = Size((int) captUndTst.get(CV_CAP_PROP_FRAME_WIDTH),
        (int) captUndTst.get(CV_CAP_PROP_FRAME_HEIGHT));
	/*
    if (refS != uTSi)
    {
        cout << "Inputs have different size!!! Closing." << endl;
        return -1;
    }
	*/
    const char* WIN_UT = "Under Test";
    const char* WIN_RF = "Reference";

    // Windows
    namedWindow(WIN_RF, CV_WINDOW_AUTOSIZE );
    namedWindow(WIN_UT, CV_WINDOW_AUTOSIZE );
    cvMoveWindow(WIN_RF, 400       ,            0);		 //750,  2 (bernat =0)
    cvMoveWindow(WIN_UT, refS.width,            0);		 //1500, 2

    cout << "Reference frame resolution: Width=" << refS.width << "  Height=" << refS.height
        << " of nr#: " << captRefrnc.get(CV_CAP_PROP_FRAME_COUNT) << endl;

    cout << "PSNR trigger value " <<
        setiosflags(ios::fixed) << setprecision(3) << psnrTriggerValue << endl;

    Mat frameReference, frameUnderTest;
    double psnrV;
    Scalar mssimV;

    while( true) //Show the image captured in the window and repeat
    {
        captRefrnc >> frameReference;
        captUndTst >> frameUnderTest;

        if( frameReference.empty()  || frameUnderTest.empty())
        {
            cout << " < < <  Game over!  > > > ";
            break;
        }

        ++frameNum;
        cout <<"Frame:" << frameNum <<"# ";

        ///////////////////////////////// PSNR ////////////////////////////////////////////////////
        psnrV = getPSNR(frameReference,frameUnderTest);					//get PSNR
        cout << setiosflags(ios::fixed) << setprecision(3) << psnrV << "dB";

        cout << endl;

        ////////////////////////////////// Show Image /////////////////////////////////////////////
        imshow( WIN_RF, frameReference);
        imshow( WIN_UT, frameUnderTest);

        c = cvWaitKey(delay);
        if (c == 27) break;
    }

    return 0;
}

 size_t Mat_total(const Mat& t) 
{
    return t.rows*t.cols;
}

double getPSNR(const Mat& I1, const Mat& I2)
{
    Mat s1;
    absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2

    Scalar s = sum(s1);         // sum elements per channel

    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

    if( sse <= 1e-10) // for small values return zero
        return 0;
    else
    {

		double  mse =sse /(double)(I1.channels() * Mat_total(I1));
        double psnr = 10.0*log10((255*255)/mse);
        return psnr;
    }
}
