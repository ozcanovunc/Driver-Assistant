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
	bool ContainsPedestrian(Mat image);
	// Returns the bounding rectangles for detected pedestrians
	vector<Rect> GetBoundingRectangles(Mat image);

private:
	HOGDescriptor hog_;
};