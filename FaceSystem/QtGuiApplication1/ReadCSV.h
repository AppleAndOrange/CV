#pragma once
#include "string"
#include "qvector.h"
#include <io.h>
#include <opencv2/opencv.hpp>  
using namespace std;


class ReadCSV
{
public:
	void ReadCsv(string, vector<cv::Mat>&, vector<int>&, vector<string>&, vector<int>&);
	ReadCSV();
	~ReadCSV();
private:
	string line, path, classLabel, name, number;
};

