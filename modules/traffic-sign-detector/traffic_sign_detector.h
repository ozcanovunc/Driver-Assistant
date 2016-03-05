#pragma once

#ifdef DEBUG	
#include <iostream>
#endif
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class TrafficSignDetector
{
public:
	// Returns the bounding rectangles for detected red traffic signs
	static vector<Rect> GetRedTrafficSigns(Mat image);
	static vector<Rect> GetBlueTrafficSigns(Mat image);

private:
	// Returns a binary image in which 
	// 1 corresponds to red area, 0 corresponds to other colors
	static Mat GetRedMask(Mat image);
	static Mat GetBlueMask(Mat image);
};