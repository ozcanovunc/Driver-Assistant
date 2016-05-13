CC = g++
CFLAGS = -g -Wall -I/usr/local/include/opencv -I/usr/local/include 

SRC = main.cpp \
rpi3.c \
modules/lane-detector/lane_detector.cpp \
modules/pedestrian-detector/pedestrian_detector.cpp \
modules/stopping-distance-calculator/stopping_distance_calculator.cpp \
modules/speed-estimator/speed_estimator.cpp \
modules/traffic-sign-detector/traffic_sign_detector.cpp

EXE = main.exe

LIB = -L/usr/local/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn -lopencv_dpm -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_plot -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core 

$(EXE) : $(SRC)
	$(CC) -o $(EXE) $(CFLAGS) $(SRC) $(LIB)
