#include "lane_detector.h"

Mat LaneDetector::mask_for_elim;

const double LaneDetector::kLaneTresh = 0.05;

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

	// Eliminate the car itself and the further traffic
	// TODO: Modifications may be needed
	for (int pi = temp.rows / 3; pi < temp.rows / 2; ++pi) {
		for (int pj = 0; pj < temp.cols; ++pj) {

			temp.at<Vec3b>(pi, pj)[0] = 255;
			temp.at<Vec3b>(pi, pj)[1] = 255;
			temp.at<Vec3b>(pi, pj)[2] = 255;
		}
	}

	LaneDetector::mask_for_elim = temp;
}

Mat LaneDetector::GetWhiteMask(Mat image, bool maskFlag) {

	Mat tresh_saturation,
		tresh_value,
		hsv;
	int sensitivity = 70;
	vector<Mat> channels;

	// Convert image to HSV, split it to channels
	if (maskFlag)
		cvtColor(LaneDetector::mask_for_elim & image, hsv, CV_BGR2HSV);
	else
		cvtColor(image, hsv, CV_BGR2HSV);
	split(hsv, channels);

	// http://i.stack.imgur.com/mkq1P.png
	tresh_saturation = channels[1] < sensitivity;
	tresh_value = channels[2] > 255 - sensitivity;

	return tresh_saturation & tresh_value;
}


vector<Vec4i> LaneDetector::GetLanes(Mat image) {

	Mat red_mask = GetWhiteMask(image, true);
	vector<Vec4i> lanes;

	erode(red_mask, red_mask, Mat(), Point(-1, -1), 1, 1, 1);
	dilate(red_mask, red_mask, Mat(), Point(-1, -1), 2, 1, 1);

	HoughLinesP(red_mask, lanes, 1, CV_PI / 360, 50, 50, 1);

	return lanes;
}

void LaneDetector::DrawLanes(Mat image, vector<Vec4i> lanes, Scalar color, int thickness) {

	for (unsigned int i = 0; i < lanes.size(); ++i)
	{
		line(image, cv::Point(lanes[i][0], lanes[i][1]),
			cv::Point(lanes[i][2], lanes[i][3]), color, thickness);
	}
}

bool LaneDetector::IsOutOfLane(Mat image)
{
	Mat temp, white_mask;
	double safety_ratio;
	image.copyTo(temp);

	temp = temp.rowRange(temp.rows * 2 / 5, temp.rows / 2);
	temp = temp.colRange(temp.cols / 3, temp.cols * 2 / 3);

	white_mask = GetWhiteMask(temp, false);

	safety_ratio = (double)(countNonZero(white_mask))
		/ (double)(white_mask.rows * white_mask.cols);

	return safety_ratio > kLaneTresh;
}