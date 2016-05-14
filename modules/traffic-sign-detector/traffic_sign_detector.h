#pragma once

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class TrafficSignDetector {

public:
	/*
	 *	Initialize with a sample image
	 */
	TrafficSignDetector(Mat image);

	/*
	 *	Returns true if it detects blue traffic signs and draws the bounding 
	 *	rectangles to matrix "out"
	 */
	bool DetectTrafficSigns(Mat in, Mat out, Scalar rect_color, int thickness);

private:
	/*
	 *	Returns a binary image in which 1 corresponds to blue area, 
	 *	0 corresponds to other colors
	 */
	Mat GetBlueMask(Mat image);

	Mat mask_for_elim_;
};