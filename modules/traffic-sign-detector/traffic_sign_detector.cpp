#include "traffic_sign_detector.h"

TrafficSignDetector::TrafficSignDetector(Mat image) {

	Mat temp = Mat::zeros(image.rows, image.cols, image.type());

	for (int pi = temp.rows / 8; pi < temp.rows / 4; ++pi) {
		for (int pj = temp.cols / 3; pj < temp.cols * 2 / 3; ++pj) {

			temp.at<Vec3b>(pi, pj)[0] = 255;
			temp.at<Vec3b>(pi, pj)[1] = 255;
			temp.at<Vec3b>(pi, pj)[2] = 255;
		}
	}

	for (int pi = temp.rows * 3 / 8; pi < temp.rows; ++pi) {
		for (int pj = 0; pj < temp.cols / 3; ++pj) {

			temp.at<Vec3b>(pi, pj)[0] = 255;
			temp.at<Vec3b>(pi, pj)[1] = 255;
			temp.at<Vec3b>(pi, pj)[2] = 255;
		}
	}

	for (int pi = temp.rows * 3 / 8; pi < temp.rows; ++pi) {
		for (int pj = temp.cols * 2 / 3; pj < temp.cols; ++pj) {

			temp.at<Vec3b>(pi, pj)[0] = 255;
			temp.at<Vec3b>(pi, pj)[1] = 255;
			temp.at<Vec3b>(pi, pj)[2] = 255;
		}
	}

	this->mask_for_elim_ = temp;
}

bool TrafficSignDetector::DetectTrafficSigns(
	Mat in, Mat out, Scalar rect_color, int thickness) {

	Mat mask = GetBlueMask(in & this->mask_for_elim_);
	vector<vector<Point> > contours;
	bool is_sign_detected = false;

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