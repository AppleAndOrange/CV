#pragma once

#ifdef WIN64
#pragma execution_character_set("utf-8")  
#endif  

#include <QWidget>
#include <qpushbutton.h>
#include <GetLib.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <mysql.h>
#include <Train.h>
#include <SelectInfo.h>
#include <Recognize.h>

class SuperModify :public QWidget
{
public:
	SuperModify(QWidget *parent = Q_NULLPTR);
	~SuperModify();
	void SelectSignal();
	void TrainSignal();
	void LibSignal();
	void addSignal();
	void DeleteLibSignal();
	void DeleteSignal();
	void AdminSignal();
	void DeleteAdminSignal();
	void RecognizeSignal();
	void LibGui();
	void Gui();
	void Connect();

private:
	QPushButton select;
	QPushButton selectAll;
	QPushButton selectId;

	QPushButton addLib;
	//QPushButton add;
	QPushButton addAdmin;
	QPushButton deleteLib;
	QPushButton deleteAdmin;
	GetMyLib lib;
	QLabel nameLable;
	QLabel passwordLable;
	QLabel userId;
	QLineEdit editId;
	QLineEdit editName;
	QLineEdit editPassword;
	QPushButton toAddLib;
	QPushButton toDeleteLib; 
	QPushButton changeAdmin;
	QPushButton train;
	QPushButton recognize;

	MYSQL *con;

	Train trainModel;

	SelectInfo quiry;

	RecognizeFace recognizeFace;
};

