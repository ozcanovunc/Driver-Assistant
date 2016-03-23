#include "lane_detector.h"

Mat LaneDetector::GetWhiteMask(Mat image) {

	Mat tresh_saturation,
		tresh_value,
		hsv;
	int sensitivity = 70;
	vector<Mat> channels;

	// Convert image to HSV, split it to channels
	cvtColor(image, hsv, CV_BGR2HSV);
	split(hsv, channels);

	// http://i.stack.imgur.com/mkq1P.png
	tresh_saturation = channels[1] < sensitivity;
	tresh_value = channels[2] > 255 - sensitivity;

	return tresh_saturation & tresh_value;
}

vector<Vec4i> LaneDetector::GetLanes(Mat image) {

	Mat red_mask = GetWhiteMask(image);
	vector<Vec4i> lanes;

	// Eliminate further traffic
	// TODO: Time consumnig operation, find other way to do it
	for (int pi = 0; pi < red_mask.rows / 4; ++pi) {
		for (int pj = 0; pj < red_mask.cols; ++pj) {

			red_mask.at<Vec3b>(pi, pj)[0] = 0;
			red_mask.at<Vec3b>(pi, pj)[1] = 0;
			red_mask.at<Vec3b>(pi, pj)[2] = 0;
		}
	}

	erode(red_mask, red_mask, Mat(), Point(-1, -1), 1, 1, 1);
	dilate(red_mask, red_mask, Mat(), Point(-1, -1), 2, 1, 1);

	HoughLinesP(red_mask, lanes, 1, CV_PI / 360, 50, 50, 1);

	return lanes;
}

void LaneDetector::PrintLanes(Mat image, vector<Vec4i> lanes, Scalar color, int thickness) {

	for (unsigned int i = 0; i < lanes.size(); ++i)
	{
		line(image, cv::Point(lanes[i][0], lanes[i][1]), 
			cv::Point(lanes[i][2], lanes[i][3]), color, thickness);
	}
}