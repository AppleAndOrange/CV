#include "Login.h"
#include "mysql.h"
#include <qmessagebox.h>
#include <string>

using namespace std;

Login::Login(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle("管理员登录");
	resize(500, 500);

	nameLable.setText("用户名：");
	nameLable.setParent(this);
	nameLable.move(100, 100);

	passwordLable.setParent(this);
	passwordLable.setText("密  码：");
	passwordLable.move(100, 150);

	editName.setParent(this);
	editName.move(200, 100);

	editPassword.setParent(this);
	editPassword.move(200, 150);

	login.setParent(this);
	login.move(200, 200);
	login.setText("登录");
	connect(&login, &QPushButton::clicked, this, &Login::AddSignal);
}


Login::~Login()
{

}
void Login::AddSignal() {
	bool flag = false;
	MYSQL *con;
	con = mysql_init(0);
	/*连接数据库*/
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
		mysql_query(con, sql);//查询数据库
		MYSQL_RES *result = mysql_store_result(con);//获得数据集
		if (result != NULL) {
			char **row;
			/*按行遍历数据集*/
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
	mysql_close(con);//关闭数据库
}
