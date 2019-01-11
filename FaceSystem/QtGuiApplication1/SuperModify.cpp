#include "SuperModify.h"
#include <windows.h>
#include <qmessagebox.h>
#include <string>
#include <cstring>
#include <qtextcodec.h>
#include <qfile.h>
#include <qdir.h>
#include <qfileinfo.h>

using namespace std;



SuperModify::SuperModify(QWidget *paren)
{
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("Unicode"));
	
	setWindowTitle("����Ա����");
	resize(700, 600);

	select.setParent(this);
	select.setText("��ѯ");
	select.move(100, 250);
	connect(&select, &QPushButton::clicked, this, &SuperModify::SelectSignal);

	train.setParent(this);
	train.setText("ģ��ѵ��");
	train.move(100, 150);
	connect(&train, &QPushButton::clicked, this, &SuperModify::TrainSignal);

	addLib.setParent(this);
	addLib.setText("����ͼ���");
	addLib.move(100, 100);
	connect(&addLib, &QPushButton::clicked, this, &SuperModify::LibSignal);


	deleteLib.setParent(this);
	deleteLib.setText("ɾ��ͼ���");
	deleteLib.move(100, 300);
	connect(&deleteLib, &QPushButton::clicked, this, &SuperModify::DeleteLibSignal);


	addAdmin.setText("���ӹ���Ա");
	addAdmin.setParent(this);
	addAdmin.move(100, 350);
	connect(&addAdmin, &QPushButton::clicked, this, &SuperModify::AdminSignal);


	deleteAdmin.setParent(this);
	deleteAdmin.setText("ɾ������Ա");
	deleteAdmin.move(100, 400);
	connect(&deleteAdmin, &QPushButton::clicked, this, &SuperModify::DeleteAdminSignal);

	recognize.setParent(this);
	recognize.setText("ʶ������");
	recognize.move(100, 200);
	connect(&recognize, &QPushButton::clicked, this, &SuperModify::RecognizeSignal);

}


SuperModify::~SuperModify()
{
	
}

/*��ѯ*/
void SuperModify::SelectSignal() {
	quiry.Select();
}
/*ģ��ѵ��*/
void SuperModify::TrainSignal() {
	trainModel.TrainModel();
	QMessageBox::information(NULL, QString::fromLocal8Bit("Train Model"), QString::fromLocal8Bit("Successfully"));
}
/*���ӡ�ɾ��������*/
void SuperModify::LibGui() {
	nameLable.setText("�û�����");
	nameLable.setParent(this);
	nameLable.move(300, 200);
	nameLable.show();

	editName.setParent(this);
	editName.move(375, 200);
	editName.show();

	toAddLib.setParent(this);
	toAddLib.move(350, 250);
	toAddLib.resize(80, 30);
	toAddLib.show();

	toDeleteLib.setParent(this);
	toDeleteLib.move(350, 250);
	toDeleteLib.resize(80, 30);
	toDeleteLib.show();
}

void SuperModify::addSignal() {
	string text = editName.text().toStdString();
	lib.GetImageLib(text);

}

void SuperModify::LibSignal() {
	LibGui();
	toDeleteLib.hide();
	changeAdmin.hide();
	passwordLable.hide();
	editPassword.hide();
	userId.hide();
	editId.hide();
	toAddLib.setText("����������");
	connect(&toAddLib, &QPushButton::clicked, this, &SuperModify::addSignal);
}
void SuperModify::DeleteSignal() {
	string text = editName.text().toStdString();
	char fileName[100];
	sprintf(fileName, "G:\\Design for graduation\\FaceRecognition\\Lib\\%s", text.c_str());
	QDir dir(fileName);
	dir.setFilter(QDir::Files);
	int fileCount = dir.count();
	/*ɾ���ļ����µ����ļ�������ļ���*/
	for (int i = 0; i < fileCount; i++)
		dir.remove(dir[i]);
	bool flag = RemoveDirectoryA(fileName);//ɾ���ļ���
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
void SuperModify::DeleteLibSignal() {
	LibGui();
	toAddLib.hide();
	changeAdmin.hide();
	passwordLable.hide();
	editPassword.hide();
	userId.hide();
	editId.hide();
	toDeleteLib.setText("ɾ��������");
	connect(&toDeleteLib, &QPushButton::clicked, this, &SuperModify::DeleteSignal);
	//QMessageBox::information(this, "success", "Delete Successfully");
}

/*���ӡ�ɾ��������Ա*/
void SuperModify::Gui() {

	userId.setText("�û�Id��");
	userId.setParent(this);
	userId.move(300, 250);
	userId.show();

	editId.setParent(this);
	editId.move(375, 250);
	editId.show();

	nameLable.setText("�û�����");
	nameLable.setParent(this);
	nameLable.move(300, 300);
	nameLable.show();

	passwordLable.setParent(this);
	passwordLable.setText("��  �룺");
	passwordLable.move(300, 350);
	passwordLable.show();

	editName.setParent(this);
	editName.move(375, 300);
	editName.show();

	editPassword.setParent(this);
	editPassword.move(375, 350);
	editPassword.show();


	changeAdmin.setParent(this);
	changeAdmin.move(350, 390);
	changeAdmin.show();
}
void SuperModify::AdminSignal() {
	Gui();
	toAddLib.hide();
	toDeleteLib.hide();
	changeAdmin.setText("���ӹ���Ա");
	connect(&changeAdmin, &QPushButton::clicked, this, &SuperModify::Connect);

}
void SuperModify::DeleteAdminSignal() {
	Gui();
	toAddLib.hide();
	toDeleteLib.hide();
	editName.hide();
	nameLable.hide();
	passwordLable.hide();
	editPassword.hide();
	userId.move(300, 350);
	editId.move(375, 350);
	changeAdmin.setText("ɾ������Ա");
	connect(&changeAdmin, &QPushButton::clicked, this, &SuperModify::Connect);

}

/*���ݿ����*/
void SuperModify::Connect() {
	con = mysql_init(0);
	if (con == NULL || !mysql_real_connect(con, "localhost", "root", "root", "student", 3306, NULL, 0)) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("It is wrong to connect"));
		return;
	}
	string text = changeAdmin.text().toStdString();
	/*ɾ������Ա*/
	if (text == "ɾ������Ա") {
		string id = editId.text().toStdString();
		int num;
		stringstream ss;
		ss << id;
		ss >> num;
		char sql[100];
		if (num != 0) {
			sprintf(sql, "delete from administrator where id = %d", num);
			int res=mysql_query(con, sql);
			if (res == 0) {
				QMessageBox::information(NULL, QString::fromLocal8Bit("OK"), QString::fromLocal8Bit("Delete successfully"));
			}
			else
			{
				QMessageBox::information(NULL, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("Delete unsuccessfully"));
			}
		}
		//change.setText(num);
		
	}
	/*���ӹ���Ա*/
	else if(text=="���ӹ���Ա")
	{
		//QMessageBox::information(NULL, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("add"));
		string num = editId.text().toStdString();
		int id;
		string name = editName.text().toStdString();
		string password = editPassword.text().toStdString();
		char sql[100];
		sscanf(num.c_str(), "%d", &id);

		sprintf(sql, "insert into administrator(id,sname,spassword) values('%d','%s','%s')",id,name.c_str(),password.c_str());
		//QMessageBox::information(NULL, QString::fromLocal8Bit("OK"), QString::fromStdString(sql));

		int res = mysql_query(con, sql);
		if (res == 0) {
			QMessageBox::information(NULL, QString::fromLocal8Bit("OK"), QString::fromLocal8Bit("Add successfully"));
		}
		else
		{
			QMessageBox::information(NULL, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("Add unsuccessfully"));
		}
	}
	mysql_close(con);
}
void SuperModify::RecognizeSignal() {
	recognizeFace.RecognizeMyself();
}