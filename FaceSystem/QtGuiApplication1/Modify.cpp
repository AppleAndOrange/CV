#include "Modify.h"
#include <windows.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <qdir.h>
#include <qfileinfo.h>


Modify::Modify(QWidget *parent)
{

	setWindowTitle("管理员操作");
	resize(500, 500);

	select.setParent(this);
	select.setText("查询");
	select.move(100, 100);
	connect(&select, &QPushButton::clicked, this, &Modify::SelectSignal);

	train.setParent(this);
	train.setText("模型训练");
	train.move(100, 150);
	connect(&train, &QPushButton::clicked, this, &Modify::TrainSignal);

	addLib.setParent(this);
	addLib.setText("增加图像库");
	addLib.move(100, 200);
	connect(&addLib, &QPushButton::clicked, this, &Modify::LibSignal);

	deleteLib.setParent(this);
	deleteLib.setText("删除图像库");
	deleteLib.move(100, 250);
	connect(&deleteLib, &QPushButton::clicked, this, &Modify::DeleteLibSignal);

	recognize.setParent(this);
	recognize.setText("识别人脸");
	recognize.move(100, 300);
	connect(&recognize, &QPushButton::clicked, this, &Modify::RecognizeSignal);
}


Modify::~Modify()
{
}


void Modify::SelectSignal() {
	inquery.Select();
}

void Modify::TrainSignal() {
	trainModel.TrainModel();
	QMessageBox::information(NULL, QString::fromLocal8Bit("Train Model"), QString::fromLocal8Bit("Successfully"));

}

void Modify::LibGui() {
	nameLable.setText("用户名：");
	nameLable.setParent(this);
	nameLable.move(200, 200);
	nameLable.show();

	editName.setParent(this);
	editName.move(275, 200);
	editName.show();

	toAddLib.setParent(this);
	toAddLib.move(250, 250);
	toAddLib.resize(80, 30);
	toAddLib.show();

	toDeleteLib.setParent(this);
	toDeleteLib.move(250, 250);
	toDeleteLib.resize(80, 30);
	toDeleteLib.show();
}

void Modify::addSignal() {
	string text = editName.text().toStdString();
	lib.GetImageLib(text);
}

void Modify::LibSignal() {
	LibGui();
	toDeleteLib.hide();
	toAddLib.setText("增加图像库");
	connect(&toAddLib, &QPushButton::clicked, this, &Modify::addSignal);
}
void Modify::DeleteSignal() {
	string text = editName.text().toStdString();
	char fileName[100];
	sprintf(fileName, "G:\\Design for graduation\\FaceRecognition\\Lib\\%s", text.c_str());
	QDir dir(fileName);
	dir.setFilter(QDir::Files);
	int fileCount = dir.count();
	/*删除文件夹下的子文件，清空文件夹*/
	for (int i = 0; i < fileCount; i++)
		dir.remove(dir[i]);
	bool flag = RemoveDirectoryA(fileName);//删除文件夹
	if (flag) {
		MYSQL *con;
		con = mysql_init(0);
		if (con == NULL || !mysql_real_connect(con, "localhost", "root", "root", "student", 3306, NULL, 0)) {
			QMessageBox::information(NULL, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("It is wrong to connect"));
			return;
		}
		else
		{
			char sql[50];
			sprintf(sql, "delete from face where s_number='%d'", atoi(text.c_str()));
			mysql_query(con, sql);
			mysql_close(con);
		}
		QMessageBox::information(this, "success", "success");
	}
	else
	{
		QMessageBox::information(this, "failure", "Failure");

	}
}
void Modify::DeleteLibSignal() {
	LibGui();
	toAddLib.hide();
	toDeleteLib.setText("删除图像库");
	connect(&toDeleteLib, &QPushButton::clicked, this, &Modify::DeleteSignal);
	//QMessageBox::information(this, "success", "Delete Successfully");
}

void Modify::RecognizeSignal() {
	recognizeFace.RecognizeMyself();
}