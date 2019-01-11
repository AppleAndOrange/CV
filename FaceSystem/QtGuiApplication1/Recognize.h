#pragma once
#include <stdio.h>
#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "ReadCSV.h"
#include <opencv2/opencv.hpp>  
#include <opencv2\face\facerec.hpp>
using namespace std;
//using namespace cv;
using namespace cv::face;

class RecognizeFace {
public:
	void RecognizeMyself();
private:
	cv::String face_cascade_name;
	cv::String eyes_cascade_name;
	ReadCSV read;
};