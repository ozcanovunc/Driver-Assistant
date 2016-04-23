#pragma once

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

enum Speed { SPD_SLOW, SPD_NORMAL, SPD_FAST, SPD_CURR };

class SpeedEstimator
{
public:
	static Speed GetSpeed(Mat image, int curr_frame);

private:
	// Returns a binary mask for asphalt color
	static Mat GetGreyMask(Mat image);

	// Reset speed per kFrameTresh
	const static int kFrameTresh;
};