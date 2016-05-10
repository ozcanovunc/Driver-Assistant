#include "lane_detector.h"

Mat LaneDetector::mask_for_elim;

const double LaneDetector::kLaneTresh = 0.05;
const double LaneDetector::kAngleTresh = 20.0;

LaneDetector::LaneDetector(Mat image) {

	Mat temp;
	image.copyTo(temp);

	for (int pi = 0; pi < temp.rows; ++pi) {
		for (int pj = 0; pj < temp.cols; ++pj) {

			temp.at<Vec3b>(pi, pj)[0] = 0;
			temp.at<Vec3b>(pi, pj)[1] = 0;
			temp.at<Vec3b>(pi, pj)[2] = 0;
		}
	}

	// Eliminate the further traffic
	for (int pi = temp.rows * 3 / 5; pi < temp.rows * 4 / 5; ++pi) {
		for (int pj = 0; pj < temp.cols; ++pj) {

			temp.at<Vec3b>(pi, pj)[0] = 255;
			temp.at<Vec3b>(pi, pj)[1] = 255;
			temp.at<Vec3b>(pi, pj)[2] = 255;
		}
	}

	LaneDetector::mask_for_elim = temp;
}

Mat LaneDetector::GetWhiteMask(Mat image, bool apply_mask) {

	Mat tresh_saturation,
		tresh_value,
		hsv;
	int sensitivity_sat = 70,
		sensitivity_val = 130;
	vector<Mat> channels;

	// Convert image to HSV, split it to channels
	if (apply_mask) {
		cvtColor(LaneDetector::mask_for_elim & image, hsv, CV_BGR2HSV);
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

	Mat white_mask = GetWhiteMask(image, true);
	vector<Vec4i> lanes;

	erode(white_mask, white_mask, Mat(), Point(-1, -1), 1, 1, 1);
	dilate(white_mask, white_mask, Mat(), Point(-1, -1), 2, 1, 1);

	HoughLinesP(white_mask, lanes, 1, CV_PI / 360, 50, 50, 1);
	return lanes;
}

void LaneDetector::DrawLanes(
	Mat image, vector<Vec4i> lanes, Scalar color, int thickness) {

	double angle;

	for (unsigned int li = 0; li < lanes.size(); ++li)
	{
		angle = atan2(lanes[li][3] - lanes[li][1], 
			lanes[li][2] - lanes[li][0]) * 180.0 / CV_PI;
		
		if (angle > kAngleTresh || angle < -kAngleTresh) {
			line(image, cv::Point(lanes[li][0], lanes[li][1]),
				cv::Point(lanes[li][2], lanes[li][3]), color, thickness);
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