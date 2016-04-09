#include "stopping_distance_calculator.h"

const double StoppingDistanceCalculator::kSafetyTresh = 0.1;

bool StoppingDistanceCalculator::IsSafe(Mat image) {

	Mat temp = GetGreyMask(image);
	double safety_ratio;

	temp = temp.rowRange(temp.rows / 4, temp.rows / 2);

	// Number of 0 pixels / number of all pixels
	safety_ratio = (double)(temp.rows * temp.cols - countNonZero(temp))
		/ (double)(temp.rows * temp.cols);

	// If the calculated safety ratio is below the threshold, the distance is fine
	return safety_ratio < kSafetyTresh;
}

Mat StoppingDistanceCalculator::GetGreyMask(Mat image) {
	
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