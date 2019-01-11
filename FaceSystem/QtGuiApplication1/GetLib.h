#pragma once

#include <stdio.h>
#include "iostream"
#include "stdlib.h"
#include "string.h"
#include <opencv2/opencv.hpp>  


using namespace std;
//using namespace cv;

class GetMyLib {
public:
	void GetImageLib(string str);
private:
	cv::String face_cascade_name;
	cv::String eyes_cascade_name;
	cv::CascadeClassifier face_cascade;
	cv::CascadeClassifier eyes_cascade;
	cv::String window_name = "Capture - Face detection";
};