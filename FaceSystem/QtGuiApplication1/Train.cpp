#include "Train.h"



Train::Train()
{
}


Train::~Train()
{
}

//Ä£ÐÍÑµÁ·
void Train::TrainModel() {
	string fn_csv = "G:\\Design for graduation\\FaceRecognition\\Lib\\at.txt";
	vector<cv::Mat> images;
	vector<int> labels;
	vector<string> names;
	vector<int> numbers;
	try
	{
		read.ReadCsv(fn_csv, images, labels, names, numbers);
	}
	catch (const std::exception&)
	{
		cout << "Error opening file:" << fn_csv << endl;
		exit(1);
	}
	if (images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(CV_StsError, error_message);
	}
	cv::Mat img = images[images.size() - 1];
	int label = labels[labels.size() - 1];
	/*cout << images.size() << endl;
	cout << labels.size() << endl;*/
	cv::Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();
	model->train(images, labels);
	model->write("G:\\Design for graduation\\FaceRecognition\\Lib\\MyFacePCAModel.xml");
	int predictedLabel = model->predict(img);
	string result_message = cv::format("Predicted class = %d / Actual class = %d.", predictedLabel, label);
	cout << result_message << endl;
	cv::waitKey(0);
}
