#include "Face.h"



Face::Face()
{
	setWindowTitle("ѡ�����");
	resize(400, 400);

	administrator.setParent(this);
	administrator.setText("���ǹ���Ա");
	administrator.move(150, 150);
	connect(&administrator, &QPushButton::clicked, this, &Face::LoginSignal);

	student.setParent(this);
	student.setText("����ѧ��");
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