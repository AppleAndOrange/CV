#include "Face.h"



Face::Face()
{
	setWindowTitle("选择身份");
	resize(400, 400);

	administrator.setParent(this);
	administrator.setText("我是管理员");
	administrator.move(150, 150);
	connect(&administrator, &QPushButton::clicked, this, &Face::LoginSignal);

	student.setParent(this);
	student.setText("我是学生");
	student.move(150, 200);
	connect(&student, &QPushButton::clicked, this, &Face::RecognizeSignal);
}


Face::~Face()
{

}

void Face::LoginSignal() {
	login.show();
}
void Face::RecognizeSignal() {
	recognize.RecognizeMyself();

}