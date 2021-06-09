#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include "yolo_v2_class.hpp"
#include "roiSetup.hpp"
#include "drawing.hpp"
#include <darknet.h>
#include <stdio.h>

using namespace std;
using namespace cv;

#define GPU
#define OPENCV

vector<string> loadClasses(string classesFile)
{
	vector<string> classes;
	ifstream ifs(classesFile.c_str());
	string line;
	while (getline(ifs, line)) classes.push_back(line);
	return classes;
}

void runDetection(VideoCapture &cap, Detector &detector, vector<string> classes)
{
	Mat frame;
	cap >> frame;
	
	RoiSetup roiSetup("Initial system setup", frame);
	roiSetup.init();

	Rect ROI(roiSetup.left_upper_corner, roiSetup.right_bottom_corner);

	while (true) {
	  	cap >> frame;
	  	if (frame.empty()) 
	  		break;
	  	//Mat image of detection area is converted to yolo input format
      	Mat croppedImage = frame(ROI);
	  	//Drawing
		shared_ptr<image_t> detImg = detector.mat_to_image_resize(croppedImage);
	  	drawGraphics(frame, detector.detect_resized(*detImg, croppedImage.cols, croppedImage.rows, 0.3), classes, roiSetup);
      	imshow("CamDetection", frame);
      	char c = (char)waitKey(10);
		if (c == 27)
			break;
	}
}

// END NEW

int main(int argc, char **argv)
{
	string classesFile = "cfg/voc.names";
	string modelConfig = "cfg/yolov4-tiny-3l_15x11__.cfg";
	string modelWeights = "cfg/yolov4-tiny-3l_15x11___best.weights";

	// Load category name
	vector<string> classes = loadClasses(classesFile);
  	printf("Object names loaded \n");

	//Load the network model, 0 is to specify the first GPU
	Detector detector(modelConfig, modelWeights, 0);

	printf("ooooooooooooo \n");
	string mode = "video";

	// Video processing
	if (mode == "video")
	{
		VideoCapture cap(argv[1]);
		runDetection(cap, detector, classes);	
		cap.release();
		destroyWindow("Running");
	}

	return 0;
}