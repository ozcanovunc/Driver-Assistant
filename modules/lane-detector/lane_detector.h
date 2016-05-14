#pragma once

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class LaneDetector {

public:
	/*
	 *	Initialize with a sample image
	 *
	 *	Mode: 0 -> If the video quality is low
	 *	Mode: 1 -> If the video quality is middle
	 *	Mode: 2 -> If the video quality is high
	 */
	LaneDetector(Mat image, int mode);

	/*
	 *	Returns true if the vehicle is out of lane
	 */
	bool IsOutOfLane(Mat image);

	vector<Vec4i> GetLanes(Mat image);
	void DrawLanes(Mat image, vector<Vec4i> lanes, Scalar color, int thickness);

private:
	/*
	 *	Returns a binary image in which 1 corresponds to white area, 0 corresponds
	 *	to other colors apply_mask flag must be true if we are detecting lanes,
	 *	false o/w
	 */
	Mat GetWhiteMask(Mat image, bool apply_mask);

	/*
	 *	Running mode
	 *
	 *	Mode: 0 -> If the video quality is low
	 *	Mode: 1 -> If the video quality is middle
	 *	Mode: 2 -> If the video quality is high
	 */
	int mode_;

	Mat mask_for_elim_;

	/*
	 *	Treshold value for checking if the vehice is in the middle of lanes
	 *	(Same as checking if the vehicle is out of lane)
	 */
	const static double kLaneTresh;

	/*
	 *	If the angle of a line is greater than kAngleTresh or less than
	 *	-kAngleTresh, then the line is considered to be vertical (they are
	 *	more likely to denote a lane)
	 */
	const static double kAngleTresh;
};