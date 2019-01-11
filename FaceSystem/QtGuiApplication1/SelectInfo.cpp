#include "SelectInfo.h"
#include <string>
#include <qmessagebox.h>
#include <fstream>
#include <qstring.h>
#include <qfiledialog.h>
#include <qdesktopservices.h>
#include<ctime> 
using namespace std;




SelectInfo::SelectInfo()
{
	

}


SelectInfo::~SelectInfo()
{
}


void SelectInfo::Select() {
	/*�������ݿ�*/
	con = mysql_init(0);
	if (con == NULL || !mysql_real_connect(con, "localhost", "root", "root", "student", 3306, NULL, 0)) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("It is wrong to connect"));
		return;
	}
	else
	{
		char face_sql[100];
		char info_sql[100];
		sprintf(face_sql, "select * from face");
		mysql_query(con, face_sql);
		MYSQL_RES *face_result = mysql_store_result(con);
		if (face_result != NULL) {
			int rows, columns;
			rows = mysql_num_rows(face_result);
			columns= mysql_num_fields(face_result);
			//count = columns - 1;
			char **row;
			ofstream file;
			//��Ҫ������ļ�   
			file.open("G:\\Design for graduation\\FaceRecognition\\Lib\\StudentInfo.csv", ios::out | ios::trunc);
			/*����ļ��ĵ�һ��*/
			file << "ѧ��" << "," << "����" << "," << "�༶";
			for (int i = 0; i < 30 && i < columns-1; i++)
				file << "," << "����";
			file << "," << "�ܼ�";
			file << endl;
			/*��ÿ�е���Ϣȡ�����������뵽�ļ���*/
			while (row=mysql_fetch_row(face_result))
			{
				int count = 0;
				string number = row[0];
				sprintf(info_sql, "select * from student where s_number='%s'", number.c_str());
				mysql_query(con,info_sql);
				MYSQL_RES *info_result = mysql_store_result(con);
				string s_name;
				string s_class;
				if (info_result) {
					int test = mysql_num_rows(info_result);
					char **info;
					info = mysql_fetch_row(info_result);
					s_name = info[1];
					s_class = info[2];
				}
				
				file << number << "," << s_name << "," << s_class;
				for (int i = 0; i < 30 && i < columns-1; i++)
					if (row[i + 1]) {
						file << "," << row[i + 1];
						count++;
					}
					else
					{
						file << ",";
						continue;
					}
				file << "," << count;
				file << endl;
			}
		}

		//�õ��ļ�·��
		//QString fileName = QFileDialog::getOpenFileName(NULL, "test", "G:\\Design for graduation\\FaceRecognition\\Lib\\StudentInfo.csv");
		//��ʾ�ļ�
		bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo("G:\\Design for graduation\\FaceRecognition\\Lib\\StudentInfo.csv").absoluteFilePath()));
		if (!success)
		{
			QMessageBox::information(NULL, "Warning", "Failure");
			return;
		}
	}

}

