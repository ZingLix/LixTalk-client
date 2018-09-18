#include "DbConnector.h"
#include <QSqlQuery>
#include <vector>

void DbConnector::connect(int id) {
	q_sql_database_.setDatabaseName(QString::number(id) + ".db");
	if(!q_sql_database_.open()) {
		throw;
	}
	init();
}

void DbConnector::init() {
	static std::vector<QString> tableList{ "chatHistory","friendList" };
	for(auto it=tableList.begin();it!=tableList.end();++it) {
		if(!checkTableExist(*it)) {
			createTable(*it);
		}
	}
}

void DbConnector::addChatHistory(int id, std::string content) {
	QString sql = QString("insert into chatHistory (sender_id,content) values (") + QString::number(id) + ",\"" + QString::fromStdString(content) + "\")";
	QSqlQuery query;
	query.exec(sql);
}

bool DbConnector::checkTableExist(QString tableName) {
	QString sql = QString("select count(*) from sqlite_master where type = 'table' name='%1'").arg(tableName);
	QSqlQuery query;
	return query.exec(sql);
}

void DbConnector::createTable(QString tableName) {
	QString str;
	if(tableName=="chatHistory") {
		str = "create table chatHistory( \
			id int auto_increment,\
			sender_id int not null, \
			content varchar not null\
			)";
	}else if(tableName=="friendList") {
		str = "create table friendList( \
			id int auto_increment,\
			user_id int not null,\
			group_id int not null\
			)";
	}
	QSqlQuery query;
	query.exec(str);
}

