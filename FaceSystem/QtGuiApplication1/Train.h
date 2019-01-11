#pragma once
#include "ReadCSV.h"
#include <opencv2/opencv.hpp>  
#include <opencv2\face\facerec.hpp>
using namespace cv::face;

class Train
{
public:
	Train();
	~Train();
	void TrainModel();
private:
	ReadCSV read;
};

