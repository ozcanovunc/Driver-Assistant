#include <opencv2/opencv.hpp>
#include <iostream>

#include "modules/speed-estimator/speed_estimator.h"
#include "modules/lane-detector/lane_detector.h"
#include "modules/pedestrian-detector/pedestrian_detector.h"
#include "modules/stopping-distance-calculator/stopping_distance_calculator.h"
#include "modules/traffic-sign-detector/traffic_sign_detector.h"

#define RED Scalar(0, 0, 255)
#define GREEN Scalar(0, 255, 0)
#define BLUE Scalar(255, 0, 0)

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{
	Mat						in, out;
	Speed					speed, currSpeed;
	vector<Vec4i>			lanes;
	vector<Rect>			pedestrians;
	LaneDetector*			lane_detector;
	PedestrianDetector*		pedestrian_detector;
	TrafficSignDetector*	sign_detector;
	int						curr_frame = 0;
	bool 					is_out_of_lane = false,
							is_distance_safe = true,
							contains_pedestrian = false;

	VideoCapture cap("1.mkv");
	//cap.set(CAP_PROP_POS_FRAMES, 10000);

	// Start from 5th min
	cap.set(CAP_PROP_POS_MSEC, 300000);
	cap >> in;

	lane_detector = new LaneDetector(in, atoi(argv[1]));
	sign_detector = new TrafficSignDetector(in);
	pedestrian_detector = new PedestrianDetector();
	namedWindow("PROJECT", WINDOW_KEEPRATIO);

	while (true) {

		cap >> in;
		in.copyTo(out);

		// Speed Estimator
		currSpeed = SpeedEstimator::GetSpeed(in, curr_frame++, atoi(argv[1]));
		if (currSpeed != SPD_CURR) {
			speed = currSpeed;
		}
		switch (speed)
		{
		case SPD_SLOW:
			putText(out, "SPEED: SLOW", Point(40, in.rows - 50), 1, 2, RED, 2);
			break;
		case SPD_NORMAL:
			putText(out, "SPEED: NORMAL", Point(40, in.rows - 50), 1, 2, RED, 2);
			break;
		case SPD_FAST:
			putText(out, "SPEED: FAST", Point(40, in.rows - 50), 1, 2, RED, 2);
			// TODO: Activate vibration motor
			break;
		}
#if 0
		// Pedestrian Detector
		if (currSpeed == SPD_FAST) {
			contains_pedestrian = pedestrian_detector->DetectPedestrians(in, out, BLUE, 2);
			if (contains_pedestrian) {
				// TODO: Activate vibration motor
			}
		}
#endif
		// Lane Detector
		lanes = lane_detector->GetLanes(in);
		is_out_of_lane = lane_detector->IsOutOfLane(in);
		lane_detector->DrawLanes(out, lanes, GREEN, 3);
		if (is_out_of_lane) {
			// TODO: Activate vibration motor
		}

		// Stopping Distance Calculator
		if (curr_frame % 20 == 0) {
			is_distance_safe = StoppingDistanceCalculator::IsSafe(in);
		}
		if (!is_distance_safe) {
			putText(out, "DISTANCE: NOT SAFE",
				Point(in.cols / 2, in.rows - 50), 1, 2, RED, 2);
			// TODO: Activate vibration motor
		}

		// Traffic Sign Detector
		sign_detector->DetectTrafficSigns(in, out, CLR_BLUE, BLUE, 2);

		imshow("PROJECT", out);
		waitKey(1);
	}

	return 0;
}