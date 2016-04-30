#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class PedestrianDetector
{
public:
	PedestrianDetector();
	// Returns true if any pedestrian has been detected and draws them into "out"
	bool DetectPedestrians(Mat in, Mat out, Scalar color, int thickness);

private:
	HOGDescriptor hog_;
};