#include "pedestrian_detector.h"

PedestrianDetector::PedestrianDetector()
{
	static vector<float> detector = HOGDescriptor::getDefaultPeopleDetector();
	hog_.setSVMDetector(detector);
}

// Returns true if any pedestrian has been detected
bool PedestrianDetector::ContainsPedestrian(Mat image)
{
	return GetBoundingRectangles(image) != vector<Rect>();
}

// Returns the bounding rectangles for detected pedestrians
vector<Rect> PedestrianDetector::GetBoundingRectangles(Mat image)
{
	vector<Rect> bounding_rects;
	hog_.detectMultiScale(image, bounding_rects, 0, Size(8, 8), Size(32, 32), 1.05, 2);
	return bounding_rects;
}