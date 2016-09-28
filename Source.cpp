#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	VideoCapture cap("video2.AVI");

	Mat image1, image2;
	Mat grayscale1, grayscale2;
	Mat absOut, thresholdImg;

	if (!cap.isOpened())
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("original", 1); // for edited video
	namedWindow("threshold", 1); // threshold video
	while (true)
	{
		bool bSuccess1 = cap.read(image1); // read a new frame from video

		if (!bSuccess1) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		cvtColor(image1, grayscale1, COLOR_BGR2GRAY); // convert to gray scale

		bool bSuccess2 = cap.read(image2); // read a new frame from video bud need little bit wait for next frame

		if (!bSuccess2) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		cvtColor(image2, grayscale2, COLOR_BGR2GRAY); // convert to gray scale
		absdiff(grayscale1, grayscale2, absOut); //input need to be in grayscale

		blur(thresholdImg, thresholdImg, Size(200, 200));	//smooth effect
		threshold(absOut, thresholdImg, 30, 255, THRESH_BINARY);	//only black and white

		erode(thresholdImg, thresholdImg, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));	//noise reduction 
		dilate(thresholdImg, thresholdImg, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));	//noise reduction 

		Moments mome = moments(thresholdImg);
		double dM01 = mome.m01;
		double dM10 = mome.m10;
		double dArea = mome.m00;

		int posX = dM10 / dArea;
		int posY = dM01 / dArea;

		if (posX >= 0 && posY >= 0)	//paint cross
		{
			line(image1, Point(posX + 30, posY), Point(posX - 30, posY), Scalar(0, 0, 255), 2); 
			line(image1, Point(posX, posY + 30), Point(posX, posY - 30), Scalar(0, 0, 255), 2);
		}

		imshow("threshold", thresholdImg); 
		imshow("original", image1);

		if (waitKey(30) == 27) //wait for 'esc' key press
		{
			cout << "esc key is pressed" << endl;
			break;
		}
	}

	return 0;
}