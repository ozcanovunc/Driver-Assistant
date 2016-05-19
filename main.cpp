#include <opencv2/opencv.hpp>

#include "modules/speed-estimator/speed_estimator.h"
#include "modules/lane-detector/lane_detector.h"
#include "modules/pedestrian-detector/pedestrian_detector.h"
#include "modules/stopping-distance-calculator/stopping_distance_calculator.h"
#include "modules/traffic-sign-detector/traffic_sign_detector.h"

#define VIBRATION_MOTOR
//#define DETECT_PEDESTRIANS

#ifdef VIBRATION_MOTOR
#include "rpi3.h"
#endif

#define RED Scalar(0, 0, 255)
#define GREEN Scalar(0, 255, 0)
#define BLUE Scalar(255, 0, 0)

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{
	Mat							in, out;
	Speed						speed, currSpeed;
	vector<Rect>				pedestrians;
	LaneDetector*				lane_detector;
	PedestrianDetector*			pedestrian_detector;
	TrafficSignDetector*		sign_detector;
	SpeedEstimator*				speed_estimator;
	StoppingDistanceCalculator* distance_calculator;
	int							curr_frame = 0,
								running_mode = atoi(argv[1]);
	bool 						is_distance_safe = true,
								contains_pedestrian = false;

#ifdef VIBRATION_MOTOR
	// Vibration motor initialization
	hardwareSetup();
#endif

	VideoCapture cap(0);
	cap >> in;
	resize(in, in, Size(480, 360));
	
	lane_detector = new LaneDetector(in, running_mode);
	sign_detector = new TrafficSignDetector(in);
	pedestrian_detector = new PedestrianDetector();
	speed_estimator = new SpeedEstimator();
	distance_calculator = new StoppingDistanceCalculator();

	while (true) {

		cap >> in;
		resize(in, in, Size(480, 360));
		in.copyTo(out);

		// Speed Estimator
		currSpeed = speed_estimator->GetSpeed(in, curr_frame++, running_mode);
		if (currSpeed != SPD_CURR) {
			speed = currSpeed;
		}
		switch (speed)
		{
			case SPD_SLOW:
				putText(out, "SPD: SLOW", 
					Point(40, in.rows - 50), 1, 2, RED, 2);
				break;
			case SPD_NORMAL:
				putText(out, "SPD: NORMAL", 
					Point(40, in.rows - 50), 1, 2, RED, 2);
				break;
			case SPD_FAST:
				putText(out, "SPD: FAST", 
					Point(40, in.rows - 50), 1, 2, RED, 2);

#ifdef VIBRATION_MOTOR
				vibrationStateChange(1);
#endif
				break;
		}

#ifdef DETECT_PEDESTRIANS
		// Pedestrian Detector
		if (currSpeed == SPD_FAST) {
			contains_pedestrian = 
				pedestrian_detector->DetectPedestrians(in, out, BLUE, 2);
			if (contains_pedestrian) {

#ifdef VIBRATION_MOTOR
				vibrationStateChange(1);
#endif
			}
		}
#endif

		// Lane Detector
		lane_detector->DrawLanes(out, lane_detector->GetLanes(in), GREEN, 5);

		// Stopping Distance Calculator
		if (curr_frame % 20 == 0) {
			is_distance_safe = distance_calculator->IsSafe(in);
		}
		if (!is_distance_safe) {
			putText(out, "DIST: NOT SAFE",
				Point(in.cols / 2, in.rows - 50), 1, 2, RED, 2);

#ifdef VIBRATION_MOTOR
			vibrationStateChange(1);
#endif
		}

		// Traffic Sign Detector
		sign_detector->DetectTrafficSigns(in, out, BLUE, 2);

		imshow("DRIVING ASSISTANT", out);
		waitKey(1);

#ifdef VIBRATION_MOTOR
		reset();
#endif

	} // Infinite loop

	return 0;
}
