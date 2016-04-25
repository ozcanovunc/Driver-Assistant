#include <opencv2\opencv.hpp>
#include <iostream>

#include "speed_estimator.h"

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{

	VideoCapture cap("output.mkv");
	Mat in;
	Speed speed;
	int curr_frame = 0;

	cap.set(CAP_PROP_POS_FRAMES, 3000);

	while (true) {

		cap >> in;

		speed = SpeedEstimator::GetSpeed(in, curr_frame++);
		if (speed != SPD_CURR) {
			switch (speed)
			{
				case SPD_SLOW:
					cout << "SLOW" << endl;
					break;
				case SPD_NORMAL:
					cout << "NORMAL" << endl;
					break;
				case SPD_FAST:
					cout << "FAST" << endl;
					break;
			}			
		}

		imshow("PROJECT", in);
		waitKey(1);
	}

	return 0;
}