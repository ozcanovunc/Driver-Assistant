#pragma once

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class LaneDetector
{
public:
	// Initialize with a sample image
	LaneDetector(Mat image);
	static bool IsOutOfLane(Mat image);
	static vector<Vec4i> GetLanes(Mat image);
	static void DrawLanes(Mat image, vector<Vec4i> lanes, Scalar color, int thickness);

private:
	// Returns a binary image in which 
	// 1 corresponds to white area, 0 corresponds to other colors
	// apply_mask flag myst be true if we are detecting lanes, false o/w
	static Mat GetWhiteMask(Mat image, bool apply_mask);
	static Mat mask_for_elim;

	// Treshold value for checking if the vehice is in the middle of lanes
	const static double kLaneTresh;
};