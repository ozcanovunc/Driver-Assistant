#include "speed_estimatior.h"

const int SpeedEstimator::kFrameTresh = 10;

Speed SpeedEstimator::GetSpeed(Mat image, int curr_frame) {

	Mat temp, left_lane, right_lane;
	static int right_lane_changes = 0;
	// Variable for checking if last frame contains lane
	static bool right_lane_exists = true;
	int non_zero_pixels;

	temp = GetGreyMask(image);
	temp = temp.rowRange(temp.rows * 3 / 8, temp.rows / 2);

	// Get right and left lanes if exist
	Rect rhs(temp.cols / 2, 0, temp.cols / 2, temp.rows);
	Rect lhs(0, 0, temp.cols / 2, temp.rows);
	left_lane = temp(lhs);
	right_lane = temp(rhs);

	non_zero_pixels = countNonZero(right_lane);

	// Gap
	if (right_lane_exists && non_zero_pixels == 0) {
		++right_lane_changes;
		right_lane_exists = false;
	}
	if (non_zero_pixels > 0) {
		right_lane_exists = true;
	}

	if (curr_frame % kFrameTresh == 0) {

		if (right_lane_changes < 3) {
			right_lane_changes = 0;
			return SPD_SLOW;
		}
		else {
			right_lane_changes = 0;
			return SPD_FAST;
		}
	}
	else {
		return SPD_CURR;
	}

}

Mat SpeedEstimator::GetGreyMask(Mat image) {
	
	Mat tresh_value,
		hsv;
	int sensitivity = 170;
	vector<Mat> channels;

	// Convert image to HSV, split it to channels
	cvtColor(image, hsv, CV_BGR2HSV);
	split(hsv, channels);

	// http://i.stack.imgur.com/mkq1P.png
	tresh_value = channels[2] < sensitivity;

	return tresh_value;
}