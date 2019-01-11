#pragma once

#ifdef WIN64
#pragma execution_character_set("utf-8")  
#endif  

#include <QWidget>
#include <qpushbutton.h>
#include <GetLib.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <Train.h>
#include <SelectInfo.h>
#include <Recognize.h>

class Modify:public QWidget
{
	Q_OBJECT

public:
	Modify(QWidget *parent = Q_NULLPTR);
	~Modify();
	void SelectSignal();
	void TrainSignal();
	void LibGui();
	void LibSignal();
	void DeleteSignal();
	void addSignal();
	void DeleteLibSignal();
	void RecognizeSignal();
private:
	QPushButton addLib;
	QPushButton deleteLib;
	QPushButton select;
	QLabel nameLable;
	QLineEdit editName;
	QPushButton toAddLib;
	QPushButton toDeleteLib;
	QPushButton train;
	GetMyLib lib;
	QPushButton recognize;


	Train trainModel;
	SelectInfo inquery;
	RecognizeFace recognizeFace;

};