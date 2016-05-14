#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class StoppingDistanceCalculator
{
public:
	bool IsSafe(Mat image);

private:
	Mat GetRedMask(Mat image);

	/*
	 *	Safety treshold for stopping distance
	 */
	const static double kSafetyTresh;
};