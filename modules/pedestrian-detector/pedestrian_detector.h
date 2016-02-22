#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class PedestrianDetector
{
public:
	PedestrianDetector();
	// Returns true if any pedestrian has been detected
	static bool ContainsPedestrian(Mat image);
	// Returns the bounding rectangles for detected pedestrians
	static vector<Rect> GetBoundingRectangles(Mat image);
};

