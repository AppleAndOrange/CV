#include "GetLib.h"
#include <windows.h>
#include <direct.h>
#include <qmessagebox.h>
#include <opencv2/opencv.hpp>  

/*
检测该文件夹是否存在，若不存在，则创建
*/
bool dirExist(const char *fileName) {
	fstream file;
	file.open(fileName, ios::in);
	if (file) {
		return true;
	}
	else
	{

		_mkdir(fileName);
		return true;
	}
}

//得到人脸库
void GetMyLib::GetImageLib(string str) {
	face_cascade_name = "E:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml";
	eyes_cascade_name = "E:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
	char fileName[100];
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Could not load video" << endl;
		return;
	}
	//int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);//帧宽度
	//int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);//帧高度
	//int fps = cap.get(CV_CAP_PROP_FPS);//帧率
	//int count = cap.get(CV_CAP_PROP_FRAME_COUNT);//帧总数

	cv::Mat frame;
	int i = 0;
	vector<cv::Rect> faces;
	//加载分类器
	face_cascade.load(face_cascade_name);
	sprintf(fileName, "G:\\Design for graduation\\FaceRecognition\\Lib\\%.50s", str.c_str());
	std::cout << fileName << endl;
	//QMessageBox::information(NULL, QString::fromStdString("Error"), QString::fromLocal8Bit(fileName));
	string folder(fileName);
	const char *file = folder.c_str();
	dirExist(file);
	while (1)
	{
		cap >> frame;
		if (frame.empty()) {
			cout << "Could not get the frame" << endl;
			return;
		}
		int row = frame.rows;
		int col = frame.cols;
		cv::Mat dst;
		dst = frame.clone();
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				dst.at<cv::Vec3b>(i, j) = frame.at<cv::Vec3b>(i, col - j - 1);
			}
		}
		imshow("frame", dst);
		cvtColor(dst, dst, CV_BGR2GRAY);
		char key = cv::waitKey(5);
		switch (key)
		{
		case 'p':
			equalizeHist(dst, dst);
			face_cascade.detectMultiScale(dst, faces, 1.1, 3, CV_HAAR_DO_ROUGH_SEARCH, cv::Size(50, 50));

			for (size_t j = 0; j < faces.size(); j++) {
				//rectangle(dst, faces[j], cv::Scalar(0, 0, 255), 2, 8, 0);
				cv::Mat faceROI = dst(faces[j]);
				cv::Mat myFace;
				if (faceROI.cols > 100) {
					i++;
					resize(faceROI, myFace, cv::Size(92, 112));
					sprintf(fileName, "G:\\Design for graduation\\FaceRecognition\\Lib\\%s\\%d.jpg",str.c_str() , i);
					imwrite(fileName, myFace);
					imshow("dst", myFace);
				}
			}
			//resize(dst, dst, Size(92, 112));
			break;
			//按住ESC退出循环
		case 27:
			return;
		default:
			break;
		}
	}
	cap.release();
	return;
}