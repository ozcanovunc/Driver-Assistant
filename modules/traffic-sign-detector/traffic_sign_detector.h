#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Color of the traffic sign
enum SignColor { CLR_BLUE, CLR_RED };

class TrafficSignDetector
{
public:
	// Initialize with a sample image
	TrafficSignDetector(Mat image);

	// Returns true if it detects any traffic signs by color
	// and draws the bounding rectangles to matrix out
	bool DetectTrafficSigns(
		Mat in, Mat out, SignColor color, Scalar rect_color, int thickness);

private:
	// Returns a binary image in which 1 corresponds to red (or blue) 
	// area, 0 corresponds to other colors
	Mat GetRedMask(Mat image);
	Mat GetBlueMask(Mat image);

	Mat mask_for_elim;
};