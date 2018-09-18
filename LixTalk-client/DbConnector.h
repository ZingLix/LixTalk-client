#ifndef LIXTALK_DBCONNECTOR
#define LIXTALK_DBCONNECTOR
#include <QSqlDatabase>

class DbConnector
{
public:
	DbConnector() {
		q_sql_database_ = QSqlDatabase::addDatabase("QSQLITE");
	}

	void connect(int id);
	void init();
	void addChatHistory(int id, std::string content);
private:
	bool checkTableExist(QString tableName);
	void createTable(QString tableName);
	QSqlDatabase q_sql_database_;
};








#endif