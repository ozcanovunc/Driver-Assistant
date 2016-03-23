#pragma once

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class LaneDetector
{
public:
	static vector<Vec4i> GetLanes(Mat image);
	static void PrintLanes(Mat image, vector<Vec4i> lanes, Scalar color, int thickness);

private:
	// Returns a binary image in which 
	// 1 corresponds to white area, 0 corresponds to other colors
	static Mat GetWhiteMask(Mat image);
};