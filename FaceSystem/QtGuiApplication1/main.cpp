#include "Login.h"
#include <QtWidgets/QApplication>
/*����ʶ��*/
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Login login;
	login.show();
	return a.exec();
}