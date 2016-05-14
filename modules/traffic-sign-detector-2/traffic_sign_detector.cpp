#include "traffic_sign_detector.h"

vector<Rect> TrafficSignDetector::GetRedTrafficSigns(Mat image) {

	Mat red_mask = GetRedMask(image);
	vector<vector<Point> > contours;
	vector<Point> approx;
	vector<Vec4i> hierarchy;
	vector<Rect> detected_signs;

	erode(red_mask, red_mask, Mat(), Point(-1, -1), 1, 1, 1);
	dilate(red_mask, red_mask, Mat(), Point(-1, -1), 4, 1, 1);

	// RETR_TREE: Creates a full family hierarchy list.
	// CV_CHAIN_APPROX_SIMPLE: compresses horizontal, vertical, 
	// and diagonal segments and leaves only their end points.
	// CV_CHAIN_APPROX_NONE: stores absolutely all the contour points.
	// Hierarchy: [Next, Previous, First_Child, Parent]
	findContours(red_mask.clone(), contours, hierarchy, RETR_TREE, 
		CV_CHAIN_APPROX_SIMPLE);

#ifdef DEBUG	
	Mat convex_hull = image.clone();
#endif

	for (int ci = 0; ci < contours.size(); ++ci) {

#ifdef DEBUG	
		vector<vector<Point> > hull(contours.size());
		convexHull(contours[ci], hull[ci], false);
		drawContours(convex_hull, hull, ci, Scalar(0, 255, 0));
#endif
		// If contour has at least one child, then draw it
		if (hierarchy[ci][2] != -1) {

			// With contour approximation will remove small curves
			// Maximum distance from contour to approximated contour: %20
			approxPolyDP(contours[hierarchy[ci][2]], approx,
				arcLength(contours[hierarchy[ci][2]], true) * 0.2, true);

			// Detected triangular traffic sign
			if (approx.size() == 3) {
				detected_signs.push_back(boundingRect(contours[ci]));
#ifdef DEBUG	
				Rect bounding_rect = boundingRect(contours[ci]);
				rectangle(image, bounding_rect.tl(), bounding_rect.br(), 
					Scalar(0, 255, 0), 2);
				cout << "triangle" << endl;
#endif
			}

			// Detected circular traffic sign - if current contour is circular and 
			// double of its childs area is greater than its own area
			if (approx.size() == 2 
				&& contours[hierarchy[ci][2]].size() * 2 > contours[ci].size()) {

				detected_signs.push_back(boundingRect(contours[ci]));
#ifdef DEBUG
				Rect bounding_rect = boundingRect(contours[ci]);
				rectangle(image, bounding_rect.tl(), bounding_rect.br(), 
					Scalar(0, 255, 0), 2);
				cout << "circle" << endl;
#endif
			}
		}
	}

#ifdef DEBUG	
	while (true) {
		imshow("mask", red_mask);
		imshow("result", image);
		imshow("hull", convex_hull);
		waitKey(30);
	}
#endif

	return detected_signs;
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