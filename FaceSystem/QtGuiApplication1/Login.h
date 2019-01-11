#pragma once

#ifdef WIN64
#pragma execution_character_set("utf-8")  
#endif  

#include <QWidget>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <Modify.h>
#include <SuperModify.h>

class Login : public QWidget
{
	Q_OBJECT
public:
	Login(QWidget *parent = Q_NULLPTR);
	~Login();
	void AddSignal();

private:
	QLabel nameLable;
	QLabel passwordLable;
	QLineEdit editName;
	QLineEdit editPassword;
	QPushButton login;
	Modify modify;
	SuperModify superModify;
};


