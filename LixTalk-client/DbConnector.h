#ifndef LIXTALK_DBCONNECTOR
#define LIXTALK_DBCONNECTOR
#include <QSqlDatabase>
#include <vector>
#include <memory>

class DbConnector
{
public:
	DbConnector() {
		q_sql_database_ = QSqlDatabase::addDatabase("QSQLITE");
	}

	void connect(int id);
	void init();
	void addChatHistory(int id, std::string content, bool recv);
	std::shared_ptr<std::vector<std::pair<std::string, bool>>> getChatHistory(int id);

private:
	bool checkTableExist(QString tableName);
	void createTable(QString tableName);
	QSqlDatabase q_sql_database_;
};








#endif