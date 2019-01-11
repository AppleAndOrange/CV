#pragma once
#ifdef WIN64
#pragma execution_character_set("utf-8")  
#endif  

#include <QWidget>
#include <qpushbutton.h>
#include <Login.h>
#include <Recognize.h>

class Face :public QWidget
{
public:
	Face();
	~Face();
	void LoginSignal();
	void RecognizeSignal();
private:
	QPushButton student;
	QPushButton administrator;
	Login login;
	RecognizeFace recognize;
};

