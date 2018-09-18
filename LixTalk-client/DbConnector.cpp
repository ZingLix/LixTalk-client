#include "DbConnector.h"
#include <QSqlQuery>
#include <QVariant>
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

void DbConnector::addChatHistory(int id, std::string content,bool recv) {
//	QString sql = QString("insert into chatHistory (sender_id,recver_id,content)\
//		 values (") + QString::number(sender_id) + ",\"" + QString::fromStdString(content) + "\")";
	
	QSqlQuery query;
	query.prepare("insert into chatHistory (sender_id,recv,content) values (?,?,?)");
	query.addBindValue(id);
	query.addBindValue(recv);
	query.addBindValue(QString::fromStdString(content));

	query.exec();


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
			recv bool not null, \
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

std::shared_ptr<std::vector<std::pair<std::string, bool>>> DbConnector::getChatHistory(int id) {
	std::shared_ptr<std::vector<std::pair<std::string, bool>>> ptr(new std::vector<std::pair<std::string, bool>>());
	QSqlQuery query;
	query.prepare("select content,recv from chatHistory where sender_id = ?");
	query.addBindValue(id);
	query.exec();
	while (query.next()) {
		ptr->push_back(std::make_pair(query.value(0).toString().toUtf8().constData(), query.value(1).toBool()));
	}
	return ptr;
}
