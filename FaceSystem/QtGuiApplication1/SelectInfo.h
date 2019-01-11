#pragma once
#include <ActiveQt/QAxObject>  
#include <mysql.h>

class SelectInfo
{
public:
	SelectInfo();
	~SelectInfo();
	void Select();


private:
	MYSQL * con;
};

