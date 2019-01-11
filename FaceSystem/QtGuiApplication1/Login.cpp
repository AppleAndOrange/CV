#include "Login.h"
#include "mysql.h"
#include <qmessagebox.h>
#include <string>

using namespace std;

Login::Login(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle("����Ա��¼");
	resize(500, 500);

	nameLable.setText("�û�����");
	nameLable.setParent(this);
	nameLable.move(100, 100);

	passwordLable.setParent(this);
	passwordLable.setText("��  �룺");
	passwordLable.move(100, 150);

	editName.setParent(this);
	editName.move(200, 100);

	editPassword.setParent(this);
	editPassword.move(200, 150);

	login.setParent(this);
	login.move(200, 200);
	login.setText("��¼");
	connect(&login, &QPushButton::clicked, this, &Login::AddSignal);
}


Login::~Login()
{

}
void Login::AddSignal() {
	bool flag = false;
	MYSQL *con;
	con = mysql_init(0);
	/*�������ݿ�*/
	if (con == NULL || !mysql_real_connect(con, "localhost", "root", "root", "student", 3306, NULL, 0)) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("It is wrong to connect"));
		return;
	}
	else
	{
		string name = editName.text().toStdString();
		string password = editPassword.text().toStdString();
		char sql[100];
		sprintf(sql, "select * from administrator");
		mysql_query(con, sql);//��ѯ���ݿ�
		MYSQL_RES *result = mysql_store_result(con);//������ݼ�
		if (result != NULL) {
			char **row;
			/*���б������ݼ�*/
			while (row = mysql_fetch_row(result))
			{
				if (name == row[1] && password == row[2]) {
					if (name == "admin"&&password == "admin") {
						superModify.show();
					}
					else
					{
						modify.show();
					}
					//close();
					flag = true;
					break;
				}
			}
			if (flag == false) {
				QMessageBox::information(NULL, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("It is wrong to login"));
			}
		}
	}
	mysql_close(con);//�ر����ݿ�
}
