#include "traffic_sign_detector.h"

bool TrafficSignDetector::DetectTrafficSigns(
	Mat in, Mat out, SignColor color, Scalar rect_color, int thickness) {

	Mat mask;
	vector<vector<Point> > contours;
	bool is_sign_detected = false;

	if (color == CLR_BLUE) {
		mask = GetBlueMask(in);
	}
	else {
		mask = GetRedMask(in);
	}

	erode(mask, mask, Mat(), Point(-1, -1), 1, 1, 1);
	dilate(mask, mask, Mat(), Point(-1, -1), 10, 1, 1);

	// Simply retrieve all the contours
	findContours(mask, contours, RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for (int ci = 0; ci < contours.size(); ++ci) {
		rectangle(out, boundingRect(contours[ci]), rect_color, thickness);
		is_sign_detected = true;
	}

	return is_sign_detected;
}

Mat TrafficSignDetector::GetRedMask(Mat image) {

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

Mat TrafficSignDetector::GetBlueMask(Mat image) {

	// Hue and sturation tresholds
	Mat tresh_hue_fst,
		tresh_hue_snd,
		tresh_saturation,
		hsv;

	vector<Mat> channels;

	// Convert image to HSV, split it to channels
	cvtColor(image, hsv, CV_BGR2HSV);
	split(hsv, channels);

	// Hue values between 200° and 300°
	tresh_hue_fst = channels[0] < 300 / 2.0f;
	tresh_hue_snd = channels[0] > 200 / 2.0f;
	tresh_saturation = channels[1] > 50;

	return (tresh_hue_fst & tresh_hue_snd) & tresh_saturation;
}