#include "ReadCSV.h"



ReadCSV::ReadCSV()
{

}


ReadCSV::~ReadCSV()
{
}
void ReadCSV::ReadCsv(string fileName, vector<cv::Mat> &images, vector<int> &labels, vector<string> &names, vector<int> &numbers) {
	ifstream file(fileName.c_str(), ifstream::in);
	if (!file) {
		string errorMessage = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, errorMessage);
	}
	while (getline(file, line)) {
		stringstream lines(line);
		//�õ�ͼ��·��
		getline(lines, path, ';');
		//�õ�ͼ����
		getline(lines, classLabel, ';');
		//�õ���������
		getline(lines, name,';');
		/*�õ�����ѧ��*/
		getline(lines, number);
		if (!path.empty() && !classLabel.empty()) {
			images.push_back(cv::imread(path, 0));
			labels.push_back(atoi(classLabel.c_str()));
			names.push_back(name.c_str());
			numbers.push_back(atoi(number.c_str()));
			//cout << path << "    " << classLabel << endl;
		}
	}
}