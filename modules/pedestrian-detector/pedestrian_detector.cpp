#include "pedestrian_detector.h"

PedestrianDetector::PedestrianDetector()
{
	static vector<float> detector = HOGDescriptor::getDefaultPeopleDetector();
	hog_.setSVMDetector(detector);
}

bool PedestrianDetector::DetectPedestrians(Mat in, Mat out, Scalar color, int thickness)
{
	vector<Rect> locations;
	hog_.detectMultiScale(in, locations, 0, Size(8, 8), Size(32, 32), 1.05, 2);

	for (int i = 0; i < locations.size(); ++i) {
		rectangle(out, locations.at(i), color, thickness);
	}

	return !locations.empty();
}