#include "Recognize.h"
#include "mysql.h"
#include "qmessagebox.h"
#include "qdatetime.h"

void RecognizeFace::RecognizeMyself() {
	const string fn_csv = "G:\\Design for graduation\\FaceRecognition\\Lib\\at.txt";
	face_cascade_name = "E:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	const string str = "G:\\Design for graduation\\FaceRecognition\\Lib\\MyFacePCAModel.xml";
	cv::VideoCapture capture(0);
	if (!capture.isOpened()) {
		cout << "It is not open" << endl;
		return;
	}
	cv::Mat frame;
	cv::Mat edges;
	cv::Mat gray;
	cv::Mat dst;
	vector<cv::Mat> images;//ѵ����ͼƬ
	vector<int> labels;//ѵ���ı�ǩ
	vector<string> names;//ѵ��������
	vector<int> numbers;//����ѧ��
	vector<bool>judge;//�ж��Ƿ����
	bool flag = false;//��ʾ���ݿ��Ƿ��Ѿ�����
	bool label_flag = false;//��ʾ�Ƿ��ҵ�������
	try
	{
		read.ReadCsv(fn_csv, images, labels, names, numbers);
	}
	catch (const std::exception&)
	{
		cout << "Error opening file:" << fn_csv << endl;
		exit(1);
	}
	cv::CascadeClassifier cascade;
	bool stop = false;
	cascade.load(face_cascade_name);
	int predict = -1;
	/*
	PCA
	*/
	cv::Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();
	/*
	������Ứ��һ��ʱ��,��ȡ�������ӿռ�
	*/
	model->read(str);
	while (!stop)
	{
		capture >> frame;
		vector<cv::Rect> faces;
		dst = frame.clone();
		/*���������ҷ�ת*/
		for (int i = 0; i < frame.rows; i++) {
			for (int j = 0; j < frame.cols; j++) {
				dst.at<cv::Vec3b>(i, j) = frame.at<cv::Vec3b>(i, frame.cols - j - 1);
			}
		}
		cvtColor(dst, gray, cv::COLOR_BGR2GRAY);
		equalizeHist(gray, gray);
		/*������⣬�õ���������*/
		cascade.detectMultiScale(gray, faces, 1.1, 3, CV_HAAR_DO_ROUGH_SEARCH, cv::Size(70, 70));
		cv::Mat face;
		cv::Point point;
		for (size_t i = 0; i < faces.size(); i++) {
			if (faces[i].height > 0 && faces[i].width > 0) {
				face = gray(faces[i]);
				point = cv::Point(faces[i].x, faces[i].y);
				rectangle(dst, faces[i], cv::Scalar(0, 255, 0), 2, 8);
			}
		}
		cv::Mat faceTest;
		
		if (face.rows >= 120) {
			resize(face, faceTest, cv::Size(92, 112));
		}
		if (!faceTest.empty()) {
			predict = model->predict(faceTest);
		}
		for (int i = 0; i < labels.size(); i++) {
			if (predict == labels[i]) {
				string name = names[i];
				putText(dst, name, point, cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255));
				if (!flag) {
					MYSQL *con;
					con = mysql_init(0);
					if (con == NULL || !mysql_real_connect(con, "localhost", "root", "root", "student", 3306, NULL, 0)) {
						QMessageBox::information(NULL, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("It is wrong to connect"));
						return;
					}
					else
					{
						//int count;
						//char sql[100];
						//string attendance = "1";
						//sprintf(sql, "select sattendance from face where s_number='%d'", numbers[i]);
						//int fail= mysql_query(con, sql);
						//MYSQL_RES *result = mysql_store_result(con);
						///*�����ѯ������ڣ����޸ĳ��ڴ���*/
						//if(result){
						//	int row_num, col_num;
						//	row_num = mysql_num_rows(result);//��ȡ����
						//	//col_num = mysql_num_fields(result);//��ȡ����
						//	if (row_num != 0) {
						//		char **row;
						//		row = mysql_fetch_row(result);
						//		count = atoi(row[0]);
						//		count++;
						//		sprintf(sql, "update face set sattendance='%s' where s_number='%d'", std::to_string(count).c_str(),numbers[i]);
						//		mysql_query(con, sql);
						//		flag = true;
						//		break;
						//	}
						//	else
						//	{
						//		/*�����ѯ��������ڣ����ڳ��ڵĴ�������������һ��*/
						//		sprintf(sql, "insert into face(s_number,sattendance) values('%d','%s')", numbers[i], attendance.c_str());
						//		mysql_query(con, sql);
						//		flag = true;
						//		break;
						//	}
						//}
						//
						char sql[100];
						int count = 0;
						/*��ȡ��ǰʱ��*/
						QDateTime current_time = QDateTime::currentDateTime();
						QString current_date = current_time.toString("yyyy.MM.dd hh:mm:ss");
						string date = current_date.toStdString();
						/*��ѯ�����������ݿ�*/
						sprintf(sql, "select * from face");
						mysql_query(con, sql);
						MYSQL_RES *result = mysql_store_result(con);
						if (result) {
							int rows, columns;
							rows = mysql_num_rows(result);
							columns= mysql_num_fields(result);
							for (int j = 0; j < rows; j++) {
								char **row;
								/*��ȡÿһ�е����ݣ�
								��������Ѿ�����������˵�ѧ�ţ�������һ�У���ʾ����ǩ��ʱ��
								����������������ڱ��û�д��ˣ������������ݿ�������һ��*/
								row = mysql_fetch_row(result);
								if (atoi(row[0]) == numbers[i]) {
									sprintf(sql, "alter table face add s_time%d varchar(50)", j + columns-1);
									mysql_query(con, sql);
									sprintf(sql, "update face set s_time%d = '%s' where s_number='%d'", j + columns-1, date.c_str(),numbers[i]);
									mysql_query(con, sql);
									flag = true;
									label_flag = true;
									break;
								}
								else
								{
									count++;
								}
							}
							/*����һ������ǩ��*/
							if (count == rows) {
								sprintf(sql, "insert into face(s_number,s_time) values('%d','%s')", numbers[i], date.c_str());
								mysql_query(con, sql);
								flag = true;
								label_flag = true;
							}
							/*������ǩ����ѭ��*/
							if (label_flag) {
								mysql_close(con);
								break;
							}

						}
					}
				}
			}
		}
		imshow("img", dst);
		if (cv::waitKey(10) == 27) {
			stop == true;
			cv::destroyWindow("img");
			capture.release();
			return;
		}

	}
}