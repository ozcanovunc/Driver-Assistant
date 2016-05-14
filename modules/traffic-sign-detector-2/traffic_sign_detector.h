#pragma once

#include <opencv2/opencv.hpp>

#ifdef DEBUG	
#include <iostream>
#endif

using namespace std;
using namespace cv;

class TrafficSignDetector {

public:
	/*
	 *	Returns the bounding rectangles for detected red traffic signs
	 */
	static vector<Rect> GetRedTrafficSigns(Mat image);

private:
	/*
	 *	Returns a binary image in which 1 corresponds to red area, 
	 *	0 corresponds to other colors
	 */
	static Mat GetRedMask(Mat image);
};