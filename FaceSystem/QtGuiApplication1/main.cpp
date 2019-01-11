#include "Login.h"
#include <QtWidgets/QApplication>
/*单人识别*/
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Login login;
	login.show();
	return a.exec();
}