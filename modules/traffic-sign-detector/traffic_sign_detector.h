#pragma once

#ifdef DEBUG	
#include <iostream>
#endif
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Color of the traffic sign
enum SignColor { CLR_BLUE, CLR_RED };

class TrafficSignDetector
{
public:
	// Returns true if it detects any traffic signs by color
	// and draws the bounding rectangles to matrix out
	static bool DetectTrafficSigns(
		Mat in, Mat out, SignColor color, Scalar rect_color, int thickness);

private:
	// Returns a binary image in which 1 corresponds to red (or blue) 
	// area, 0 corresponds to other colors
	static Mat GetRedMask(Mat image);
	static Mat GetBlueMask(Mat image);
};