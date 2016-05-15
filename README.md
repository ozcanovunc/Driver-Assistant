# Driver Assistant

### SOFTWARE
----------
#### ► Color Filtering

- This method has been used in most of the modules. 
- In order to detect some desired color from an image, some kind of a filtering mechanism have been implemented.
- This way, it would be possible to get a binary image which, only pixels of desired color are shown.
- This is achieved by converting from RGB colormap to HSV. After that, some thresholds have been applied to HSV values.

#### ► Lane Detector

- Input image have been cropped %40 from the top and %20 from the bottom.
- White color mask have been applied to cropped image.
- After running Hough line detection algorithm, horizontal lines were removed.
- The vertical ones have been drawn.

> <img src="https://github.com/ozcanovunc/Driver-Assistant/blob/master/sample-images/lane-detector-1.png" width="300">

#### ► Pedestrian Detector

- OpenCV's default people detector has been used.

#### ► Speed Estimator

- Since we have only one camera, estimating the approximate speed of the vehicle is hard.
- After getting the input image, the regions that we expect to detect lanes have been cropped.
- Then we calculate the frequency of lanes passing these regions.
- According to that frequency, we estimate the speed of the vehicle.

#### ► Stopping Distance Calculator

- Input image have been cropped regarding the front of the vehicle where it is not safe to detect any car in that region.
- Red color mask have been applied to cropped image, hoping to detect taillights.
- If we detect taillights, the distance between us and that car is not safe.

> <img src="https://github.com/ozcanovunc/Driver-Assistant/blob/master/sample-images/stopping-distance-calculator-1.png" width="300">

#### ► Traffic Sign Detector

- We are able to detect only blue colored signs, with a very high possibility.
- Regarding the performance, only blue color mask have been applied to the input image.
- After getting the result of contour detection, detected regions are considered to be traffic signs.
- In this [link](https://github.com/ozcanovunc/Driver-Assistant/tree/master/modules/traffic-sign-detector-2), more robust algorithm has been implemented for detecting only red traffic signs. (If the quality of images are high)

> <img src="https://github.com/ozcanovunc/Driver-Assistant/blob/master/sample-images/traffic-sign-detector-1.png" width="300">

> <img src="https://github.com/ozcanovunc/Driver-Assistant/blob/master/sample-images/traffic-sign-detector-3.png" width="300">

### HARDWARE
----------
- The entire software runs on Raspberry Pi 3 computer.
- In order to give feedback to the user, a vibration motor has been used as well.
- When the river drives too fast or the stopping distance is not safe, the motor vibrates.

### HOW TO RUN
----------
- Install OpenCV 3.1 on your Raspberry.
- Install WiringPi library on your Raspberry.
- ```make```
- ```./main.exe [RUNNING_MODE]```
- Where [RUNNING_MODE] may get the values of 0, 1 or 2 which denotes the quality of the input video. (2 means high quality)
