#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

enum Speed { SPD_SLOW, SPD_NORMAL, SPD_FAST, SPD_CURR };

class SpeedEstimator
{
public:
	// Mode: 0 -> If the video quality is low
	// Mode: 1 -> If the video quality is middle
	// Mode: 2 -> If the video quality is high
	static Speed GetSpeed(Mat image, int curr_frame, int mode);

private:
	static Mat GetWhiteMask(Mat image, int mode);

	/* 
		Number of frames needed in order to estimate the speed of the vehicle
	*/
	const static int kFrameTresh;

	/* 
		Maximum number of gaps detected between lanes per kFrameTresh
		in order to decide if the vehicle is going too fast or not
		If the number of gaps below this treshold, vehicle is considered
		to have normal speed
	*/
	const static int kGapTreshFast;
};