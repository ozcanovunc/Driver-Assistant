#include "lane_detector.h"

const double LaneDetector::kLaneTresh = 0.05;
const double LaneDetector::kAngleTresh = 20.0;

LaneDetector::LaneDetector(Mat image, int mode) {

	Mat temp = Mat::zeros(image.rows, image.cols, image.type());

	// Search for lanes in rows between %40 and %80
	for (int pi = temp.rows * 2 / 5; pi < temp.rows * 4 / 5; ++pi) {
		for (int pj = 0; pj < temp.cols; ++pj) {

			temp.at<Vec3b>(pi, pj)[0] = 255;
			temp.at<Vec3b>(pi, pj)[1] = 255;
			temp.at<Vec3b>(pi, pj)[2] = 255;
		}
	}

	this->mask_for_elim_ = temp;
	this->mode_ = mode;
}

Mat LaneDetector::GetWhiteMask(Mat image, bool apply_mask) {

	Mat			tresh_saturation,
				tresh_value,
				hsv;
	vector<Mat> channels;
	int			sensitivity_sat,
				sensitivity_val;

	if (this->mode_ == 0) {
		sensitivity_sat = 70;
		sensitivity_val = 130;
	}
	else if (this->mode_ == 1) {
		sensitivity_sat = 70;
		sensitivity_val = 115;
	}
	else {
		sensitivity_sat = 70;
		sensitivity_val = 70;
	}

	// Convert image to HSV, split it to channels
	if (apply_mask) {
		cvtColor(this->mask_for_elim_ & image, hsv, CV_BGR2HSV);
	}
	else {
		cvtColor(image, hsv, CV_BGR2HSV);
	}

	split(hsv, channels);

	// http://i.stack.imgur.com/mkq1P.png
	tresh_saturation = channels[1] < sensitivity_sat;
	tresh_value = channels[2] > 255 - sensitivity_val;

	return tresh_saturation & tresh_value;
}

vector<Vec4i> LaneDetector::GetLanes(Mat image) {

	Mat white_mask = this->GetWhiteMask(image, true);
	vector<Vec4i> lanes;

	erode(white_mask, white_mask, Mat(), Point(-1, -1), 1, 1, 1);
	dilate(white_mask, white_mask, Mat(), Point(-1, -1), 2, 1, 1);

	HoughLinesP(white_mask, lanes, 1, CV_PI / 360, 50, 50, 1);
	return lanes;
}

void LaneDetector::DrawLanes(
	Mat image, vector<Vec4i> lanes, Scalar color, int thickness) {

	double angle;

	// Eliminate some of the lanes which are not vertical
	for (unsigned int li = 0; li < lanes.size(); ++li)
	{
		angle = atan2(lanes[li][3] - lanes[li][1], 
			lanes[li][2] - lanes[li][0]) * 180.0 / CV_PI;
		
		if (angle > kAngleTresh || angle < -kAngleTresh) {
			line(image, Point(lanes[li][0], lanes[li][1]), 
				Point(lanes[li][2], lanes[li][3]), color, thickness);
		}
	}
}

bool LaneDetector::IsOutOfLane(Mat image)
{
	Mat temp, white_mask;
	double safety_ratio;
	image.copyTo(temp);

	temp = temp.rowRange(temp.rows * 3 / 5, temp.rows * 4 / 5);
	temp = temp.colRange(temp.cols / 3, temp.cols * 2 / 3);

	white_mask = GetWhiteMask(temp, false);

	safety_ratio = (double)(countNonZero(white_mask))
		/ (double)(white_mask.rows * white_mask.cols);

	return safety_ratio > kLaneTresh;
}