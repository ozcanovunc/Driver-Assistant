#include "speed_estimator.h"

const int SpeedEstimator::kFrameTresh = 20;
const int SpeedEstimator::kGapTreshFast = 4;

Speed SpeedEstimator::GetSpeed(Mat image, int curr_frame) {

	Mat temp, left_lane, right_lane;
	static int right_lane_changes = 0;
	static int left_lane_changes = 0;
	// Variable for checking if last frame contains lane
	static bool right_lane_exists = true;
	static bool left_lane_exists = true;
	int non_zero_pixels,
		avg_lane_changes;

	temp = GetWhiteMask(image);
	temp = temp.rowRange(temp.rows * 2 / 3, temp.rows * 4 / 5);

	// Get right and left lanes if exist (ROI)
	Rect rhs(temp.cols / 2, 0, temp.cols / 2, temp.rows);
	Rect lhs(0, 0, temp.cols / 2, temp.rows);
	left_lane = temp(lhs);
	right_lane = temp(rhs);

	// If there's a lane, this should return a non zero value
	non_zero_pixels = countNonZero(right_lane);

	// Calculate the gaps between lanes (right)
	if (right_lane_exists && non_zero_pixels == 0) {
		++right_lane_changes;
		right_lane_exists = false;
	}
	if (non_zero_pixels > 0) {
		right_lane_exists = true;
	}

	// If there's a lane, this should return a non zero value
	non_zero_pixels = countNonZero(left_lane);

	// Calculate the gaps between lanes (left)
	if (left_lane_exists && non_zero_pixels == 0) {
		++left_lane_changes;
		left_lane_exists = false;
	}
	if (non_zero_pixels > 0) {
		left_lane_exists = true;
	}

	if (curr_frame % kFrameTresh == 0) {

		avg_lane_changes = right_lane_changes > left_lane_changes ? 
			right_lane_changes : left_lane_changes;

#if DEBUG
		cout << avg_lane_changes << endl;
#endif

		if (avg_lane_changes == 0) {
			right_lane_changes = 0;
			left_lane_changes = 0;
			return SPD_SLOW;
		}
		else if (avg_lane_changes < kGapTreshFast){
			right_lane_changes = 0;
			left_lane_changes = 0;
			return SPD_NORMAL;
		}
		else {
			right_lane_changes = 0;
			left_lane_changes = 0;
			return SPD_FAST;
		}
	}
	else {
		return SPD_CURR;
	}
}

Mat SpeedEstimator::GetWhiteMask(Mat image) {

	Mat tresh_saturation,
		tresh_value,
		hsv;
	int sensitivity_sat = 70,
		sensitivity_val = 130;
	vector<Mat> channels;

	// Convert image to HSV, split it to channels
	cvtColor(image, hsv, CV_BGR2HSV);

	split(hsv, channels);

	// http://i.stack.imgur.com/mkq1P.png
	tresh_saturation = channels[1] < sensitivity_sat;
	tresh_value = channels[2] > 255 - sensitivity_val;

	return tresh_saturation & tresh_value;
}