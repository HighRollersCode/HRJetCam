#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <stdio.h>
using namespace cv;
void Test1();
void Test2();


int main()
{
    Test2();
}

void Test1()
{
    cv::Mat img;
    cv::Mat dst;
    cv::Sobel(img,dst,CV_32F,1,1);
    cv::VideoCapture input(0);
    /*
    cv::VideoWriter output(
	"cars_sobel.avi",
	CV_FOURCC('X','V','I','D'),
	30,
	cv::Size(input.get(CV_CAP_PROP_FRAME_WIDTH),
	input.get(CV_CAP_PROP_FRAME_HEIGHT)));
    */

    for(;;)
    {
	if(!input.read(img))
	    break;

	cv::Sobel(img, dst, CV_8U, 1, 1);
	//output.write(dst);

	cv::imshow("img",dst);
	char c = cv::waitKey(10);

	if( c == ' ')
	    break;
    }
    
}

void Test2()
{
    
    IplImage *img;
    cv::Mat imgHSV;
    cv::Mat imgthresh;
    cv::Mat imgDilated;
    cv::Mat imgContours;
    cv::vector<cv::vector<cv::Point> > contours;
    cv::vector<cv::vector<cv::Point> > goodcontours;
    cv::vector<cv::Vec4i> hierarchy;
    cv::RNG rng(12345);

    int c;
    CvCapture *cv_cap = cvCaptureFromCAM(0);
    cvSetCaptureProperty(cv_cap, CV_CAP_PROP_FRAME_WIDTH,320);
    cvSetCaptureProperty(cv_cap, CV_CAP_PROP_FRAME_HEIGHT,240);
    cvNamedWindow("Video",0);
    
    char buffer[10];
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.5, 1.5,1,5,1);
    double t1;
    //341 is a blankity
    for(;;)
    {
	double t = cv::getTickCount();
	img = cvQueryFrame(cv_cap);

	if (img != 0)
	{
	    int d = 0;
	    cv::cvtColor(cv::cvarrToMat(img),imgHSV,cv::COLOR_BGR2HSV);
	    cv::inRange(imgHSV,cv::Scalar(40,167,193),cv::Scalar(119,255,255),imgthresh);
	    cv::dilate(imgthresh, imgDilated, cv::Mat(), cv::Point(-1, -1), 2);
	    cv::findContours( imgDilated, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
	    for( int i = 0; i < contours.size(); i++)
	    {
		if(cv::contourArea(contours[i])<1000)
		{
		    contours.erase(contours.begin() + i);
		    d++;
		}
		else
		{
		    //cv::Scalar color = cv::Scalar( rng.uniform(0, 1), rng.uniform(0, 1), rng.uniform(254, 255) );
		    cv::Scalar color = cv::Scalar( 0, 0, 255);
		    cv::drawContours( cv::cvarrToMat(img), contours, i, color, 2, 8, hierarchy, 0, cv::Point(0, 0));
		}
	    }

	//    sprintf(buffer, "framerate %f",1 / t1 );
	    sprintf(buffer, "d: %d, fps: %f",d,1/t1);
	//    cv::putText(img, buffer, cv::Point(0, 50), &font, cvScalar(255));
	    cvPutText(img, buffer, cv::Point(0, 50), &font, cvScalar(0,255,0));
	    cv::imshow("Video",cv::cvarrToMat(img));
	}
	 c = cvWaitKey(1);
	if (c== 27)
	    break;
	
	t1 = ((double)cv::getTickCount()-t)/cv::getTickFrequency();


    }

    cvReleaseCapture(&cv_cap);
    cvDestroyWindow("Video");
}

