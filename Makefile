CC = g++
CFLAGS = -g -Wall

SRC = main.cpp \
modules/lane-detector/lane_detector.cpp \
modules/pedestrian-detector/pedestrian_detector.cpp \
modules/stopping-distance-calculator/stopping_distance_calculator.cpp

EXE = main.exe

# C:\opencv\build\x86\install\x86\mingw\lib
LIB = -llibopencv_calib3d310 \
-llibopencv_core310 \
-llibopencv_features2d310 \
-llibopencv_flann310 \
-llibopencv_highgui310 \
-llibopencv_imgcodecs310 \
-llibopencv_imgproc310 \
-llibopencv_ml310 \
-llibopencv_objdetect310 \
-llibopencv_photo310 \
-llibopencv_shape310 \
-llibopencv_stitching310 \
-llibopencv_superres310 \
-llibopencv_video310 \
-llibopencv_videoio310 \
-llibopencv_videostab310

OPENCV = -I"C:\opencv\build\x86\install\include" \
-L"C:\opencv\build\x86\install\x86\mingw\lib"

$(EXE) : $(SRC)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC) $(OPENCV) $(LIB)