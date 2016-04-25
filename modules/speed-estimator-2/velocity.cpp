#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

string getSpeedText(double speed);

int main(int argc, char** argv)
{

	Mat firstFrame;
	Mat secondFrame;

	/** Input file is taken from command line. 
	 *  Change this to get input from camera.
	 */
	VideoCapture cap(argv[1]);
	if (!cap.isOpened()) {
		cout << "File couldn't be opened." << endl;
		return -1;
	}

	namedWindow("Original Image", 0);             //Window to show original image
	namedWindow("Lukas-Kanade Optical Flow", 0);  //Window to show Lukas-Kanade optical flow
	cap.set(CAP_PROP_POS_FRAMES, 1000);           //Start processing video from 1000th frame.
												  //Change this to zero or delete this line 
												  //if you want to start from the beginning of video.

	int counter = 0;
	double distance = 0.0;

	Mat firstFrameOriginal, secondFrameOriginal;
	
	while (true) {

		cap >> firstFrameOriginal;  //Capture a frame
		cap >> secondFrameOriginal; //Capture the next frame

		firstFrame = firstFrameOriginal/*(Rect(10, 10, 300, 300))*/;
		secondFrame = secondFrameOriginal/*(Rect(10, 10, 300, 300))*/;

		/* Convert to grayscale. */
		cvtColor(firstFrame, firstFrame, CV_BGR2GRAY);
		cvtColor(secondFrame, secondFrame, CV_BGR2GRAY);

		/* This may help with detecting better optical flow points, but I do not guarantee it. */
		/*threshold(firstFrame, firstFrame, 100, 255, 3);
		threshold(secondFrame, secondFrame, 100, 255, 3);*/

		/* Resize frames to 400x400 */
		resize(firstFrame, firstFrame, Size(400, 400));
		resize(secondFrame, secondFrame, Size(400, 400));

		/**
		 * Take part of images. If image width is x and image height is y,
		 * then image size will be (width/2)x(height/2). (width/4, height/4) coordinates
		 * of the old frames will be the new (0,0) points of the new images (upper left corner)
		 */
		firstFrame = firstFrame(Rect(firstFrame.cols / 4, firstFrame.rows / 4, firstFrame.rows / 2, firstFrame.cols / 2));
		secondFrame = secondFrame(Rect(secondFrame.cols / 4, secondFrame.rows / 4, secondFrame.rows / 2, secondFrame.cols / 2));


		/* Took this from an example on the internet. May or may not help.*/
		blur(firstFrame, firstFrame, Size(3, 3));
		blur(secondFrame, secondFrame, Size(3, 3));

		/* Canny may be useful, I am not sure. */
		/*Canny(firstFrame, firstFrame, 50, 150, 3);
		Canny(secondFrame, secondFrame, 50, 150, 3);*/

		Size opticalFlowFrameSize = firstFrame.size();
		Mat opticalFlowFrame(opticalFlowFrameSize, 1);

		int windowSize = 50;                          //window size
		int maxCorners = 100;                         //maximum corner size that goodFeaturesToTrack function should find
		double qualityLevel = 0.01;                   //Minimum corner quality factor. For example, if a corner has a quality measure 1500,
									                  //then all corners which have a quality of 15 (1500*0.01) will be rejected.
		double minDistance = 5.0;                     //Minimum possible distance between returned corners (Euclidean distance)
		std::vector<cv::Point2f> firstFrameCorners;   //Corners which are returned from the first image
		firstFrameCorners.reserve(maxCorners);        //Initialize vector with the size of maxCorners
		std::vector<cv::Point2f> secondFrameCorners;  //Corners which are returned from the second image
		secondFrameCorners.reserve(maxCorners);       //Initialize vector with the size of maxCorners


		/* Find suitable corners for optical flow to track. Found points are stored in firstFrameCorners and secondFrameCorners vectors*/
		goodFeaturesToTrack(firstFrame, firstFrameCorners, maxCorners, qualityLevel, minDistance, cv::Mat());
		goodFeaturesToTrack(secondFrame, secondFrameCorners, maxCorners, qualityLevel, minDistance, cv::Mat());

		/*cornerSubPix(firstFrame, firstFrameCorners, Size(windowSize, windowSize), Size(-1, -1),
			TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
		cornerSubPix(secondFrame, secondFrameCorners, Size(windowSize, windowSize), Size(-1, -1),
			TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));*/

		std::vector<uchar> foundFeatures;  //output status vector
		foundFeatures.reserve(maxCorners); //Initialize vector with the size of maxCorners
		std::vector<float> featureErrors;  //output vector of errors
		featureErrors.reserve(maxCorners); //Initialize vector with the size of maxCorners

		/* Lukas-Kanade version of Optical Flow */
		calcOpticalFlowPyrLK(firstFrame, secondFrame, firstFrameCorners, secondFrameCorners, foundFeatures, featureErrors,
			Size(windowSize, windowSize), 5,
			cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3), 0);

		double maxDist = 0.0;
		int count = 0;

		for (int i = 0; i < foundFeatures.size(); i++) {
			
			Point p0(ceil(firstFrameCorners[i].x), ceil(firstFrameCorners[i].y));
			Point p1(ceil(secondFrameCorners[i].x), ceil(secondFrameCorners[i].y));
			
			/* Show found points on screen. */
			line(opticalFlowFrame, p0, p1, CV_RGB(0, 168, 107), 1);

			/* Find the distance between the original point and its new position */
			/* In my opinion, we need to find the change along Z-axis to find a more accurete velocity,
			   but I am not sure how to do that. It doesn't seem to be an easy task, unfortunately.
			   This sqrt operation merely finds the distance with the help of x,y points. Therefore,
			   the distance and the speed related to this will be higher than what it should be at
			   road curves, or while turning a corner due to the changes along the x-axis.
			*/
			double dist = sqrt(pow(p0.x - p1.x, 2) + pow(p0.y - p1.y, 2));

			/*Ignore distances below 1 */
			if (dist > 1) {
				maxDist += dist; //total distance between all points (maxCorners)
				++count;
			}
		}

		int limit = 50; //Show speed at every limit-value frames

		if (counter == limit) {
			cout << "D: " << distance / limit << endl; //average the distance value by limit
			
			/* Average velocity. (Very simple and straightforward implementation. Prone to many incorrect values. */
			cout << "Dist Speed: " << (distance / limit / count) * cap.get(CAP_PROP_FPS) << endl;

			/* Just a text version of the speed. Check getSpeedText for more info. */
			cout << "Speed: " << getSpeedText((distance / limit / count) * cap.get(CAP_PROP_FPS)) << endl;
			counter = 0;
			distance = 0.0;
		}
		else {
			distance += maxDist;
			++counter;
		}

		/* Show original frame */
		imshow("Original Image", firstFrame);
		//imshow("ImageB", secondFrame);

		/* Show optical flow frame */
		imshow("Lukas-Kanade Optical Flow", opticalFlowFrame);

		/*Wait at least 1 milisecond to prevent program using 100% of CPU, thus freezing.*/
		cvWaitKey(1);
	}

	return 0;
}

/**
 * All values here are arbitrary values.
 * When the algroithm that finds the value of parameter speed is changed,
 * it will be necessary to adjust these values as vell.
 */
string getSpeedText(double speed) {
	if (speed == NAN) return "";
	if (speed < 40) return "Slow";
	if (speed < 90) return "Normal";
	if (speed < 200) return "Fast";
	return "Very Fast";
}