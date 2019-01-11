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
	vector<cv::Mat> images;//训练的图片
	vector<int> labels;//训练的标签
	vector<string> names;//训练的姓名
	vector<int> numbers;//该生学号
	vector<bool>judge;//判断是否检查过
	bool flag = false;//表示数据库是否已经连接
	bool label_flag = false;//表示是否找到该人脸
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
	在这里会花费一段时间,读取特征脸子空间
	*/
	model->read(str);
	while (!stop)
	{
		capture >> frame;
		vector<cv::Rect> faces;
		dst = frame.clone();
		/*将人脸左右翻转*/
		for (int i = 0; i < frame.rows; i++) {
			for (int j = 0; j < frame.cols; j++) {
				dst.at<cv::Vec3b>(i, j) = frame.at<cv::Vec3b>(i, frame.cols - j - 1);
			}
		}
		cvtColor(dst, gray, cv::COLOR_BGR2GRAY);
		equalizeHist(gray, gray);
		/*人脸检测，得到人脸序列*/
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
						///*如果查询结果存在，则修改出勤次数*/
						//if(result){
						//	int row_num, col_num;
						//	row_num = mysql_num_rows(result);//获取行数
						//	//col_num = mysql_num_fields(result);//获取列数
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
						//		/*如果查询结果不存在，则在出勤的次数那里新增加一条*/
						//		sprintf(sql, "insert into face(s_number,sattendance) values('%d','%s')", numbers[i], attendance.c_str());
						//		mysql_query(con, sql);
						//		flag = true;
						//		break;
						//	}
						//}
						//
						char sql[100];
						int count = 0;
						/*获取当前时间*/
						QDateTime current_time = QDateTime::currentDateTime();
						QString current_date = current_time.toString("yyyy.MM.dd hh:mm:ss");
						string date = current_date.toStdString();
						/*查询人脸考勤数据库*/
						sprintf(sql, "select * from face");
						mysql_query(con, sql);
						MYSQL_RES *result = mysql_store_result(con);
						if (result) {
							int rows, columns;
							rows = mysql_num_rows(result);
							columns= mysql_num_fields(result);
							for (int j = 0; j < rows; j++) {
								char **row;
								/*获取每一行的数据，
								如果该行已经存在了这个人的学号，则增加一列，显示本次签到时间
								如果遍历玩人脸考勤表均没有此人，则向人脸数据库中增加一行*/
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
							/*增加一行人脸签到*/
							if (count == rows) {
								sprintf(sql, "insert into face(s_number,s_time) values('%d','%s')", numbers[i], date.c_str());
								mysql_query(con, sql);
								flag = true;
								label_flag = true;
							}
							/*跳出标签遍历循环*/
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