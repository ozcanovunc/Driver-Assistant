#include "stopping_distance_calculator.h"

const double StoppingDistanceCalculator::kSafetyTresh = 0;

bool StoppingDistanceCalculator::IsSafe(Mat image) {

	Mat temp = GetRedMask(image);
	double safety_ratio;

	temp = temp.rowRange(temp.rows / 3, temp.rows / 2); // TODO: Optimize this
	temp = temp.colRange(temp.cols * 2 / 5, temp.cols * 3 / 5);

	// Number of non zero pixels / number of all pixels, percentage of red color
	safety_ratio = (double)(countNonZero(temp))
		/ (double)(temp.rows * temp.cols);

	// If the calculated safety ratio is higher than the threshold, the distance is not safe
	return safety_ratio <= kSafetyTresh;
}

Mat StoppingDistanceCalculator::GetRedMask(Mat image) {
	
	// Hue and sturation tresholds
	Mat tresh_hue_fst,
		tresh_hue_snd,
		tresh_saturation,
		hsv;

	vector<Mat> channels;

	// Convert image to HSV, split it to channels
	cvtColor(image, hsv, CV_BGR2HSV);
	split(hsv, channels);

	// Hue values between 0° to 25° and 335° to 360°
	tresh_hue_fst = channels[0] < 25 / 2.0f;
	tresh_hue_snd = channels[0] > 335 / 2.0f;
	tresh_saturation = channels[1] > 50;

	return (tresh_hue_fst | tresh_hue_snd) & tresh_saturation;
}