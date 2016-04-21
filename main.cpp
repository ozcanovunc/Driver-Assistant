#include <opencv2\opencv.hpp>
#include <iostream>

#include "modules/lane-detector/lane_detector.h"
#include "modules/pedestrian-detector/pedestrian_detector.h"
#include "modules/stopping-distance-calculator/stopping_distance_calculator.h"

#define LANE_COLOR Scalar(0, 255, 0)

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{
	Mat in;
	vector<Vec4i> lanes;
	vector<Rect> pedestrians;
	bool 	is_out_of_lane = false,
			is_distance_safe = true,
			contains_pedestrian = false;

	in = imread("1.png");

	LaneDetector* lane_detector = new LaneDetector(in);
	PedestrianDetector* pedestrian_detector = new PedestrianDetector();

	while (true) {

		in = imread("1.png");

		// Stopping Distance Calculator Module
		is_distance_safe = StoppingDistanceCalculator::IsSafe(in);
		if (!is_distance_safe) {
			cout << "NOT SAFE" << endl;
		}
#if 0
		// Pedestrian Detector Module
		contains_pedestrian = pedestrian_detector->ContainsPedestrian(in);
		if (contains_pedestrian) {
			pedestrians = pedestrian_detector->GetBoundingRectangles(in);
		}
#endif
		// Lane Detector Module
		lanes = lane_detector->GetLanes(in);
		is_out_of_lane = lane_detector->IsOutOfLane(in);
		lane_detector->DrawLanes(in, lanes, LANE_COLOR, 3);


		imshow("PROJECT", in);
		waitKey(10);	
	}

	return 0;
}