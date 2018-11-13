#include "client.h"
#include "message.h"

client::client(const QString& addr, int port):soc_(this) {
	soc_.connectToHost(addr, port);
}

client::client(std::string addr, int port) : soc_(this) {
	soc_.connectToHost(QString::fromStdString(addr), port);
}

void client::login(const QString& username, const QString& password) {
	message m;
	m.add("type", 0);
	m.add("recver_id", 0);
	m.add("username", username);
	m.add("password", password);
	std::string mmm = m.getString();

	connect(&soc_, SIGNAL(readyRead()), this, SLOT(loginExec()));
	soc_write(m.getString());

}

void client::reg_accout(const QString& username, const QString& password) {
	message m;
	m.add("recver_id", 0);
	m.add("type", 1);
	m.add("username", username);
	m.add("password", password);
	soc_write(m.getString());
	connect(&soc_, SIGNAL(readyRead()), this, SLOT(registerExec()));
}

void client::send(int recver_id, const QString& msg) {
	message m;
	m.add("type", 9);
	m.add("recver_id", recver_id);
	m.add("sender_id", user_id);
	m.add("content", msg);
	std::string d = m.getString();
	db_.addChatHistory(recver_id, msg.toUtf8().constData(), false);
	soc_write(m.getString());
}

client::~client() {
	emit clientDestroyed();
}

void client::loginExec() {
	QString msg = soc_.readAll();
	message m(msg.toUtf8().constData());
	if(m.getInt("type")==0) {
		if(m.getInt("result")==1) {
			user_id = m.getInt("recver_id");
			//disconnect(this, SLOT(registerExec()));
			disconnect(&soc_, SIGNAL(readyRead()), this, SLOT(loginExec()));
			connect(&soc_, SIGNAL(readyRead()), this, SLOT(newMsgExec()));
			db_.connect(user_id);
			if (db_.checkNew()) pullMsg();
			emit loginSuccess(user_id);
		}else {
			emit loginFailure("unknown error!");
			soc_.close();
		}
	}else {
		emit loginFailure(m.getString("content"));
		soc_.close();
	}
}

void client::registerExec() {
	QString msg = soc_.readAll();
	message m(msg.toUtf8().constData());
	if (m.getInt("type") == 3) {
		if (m.getInt("result") == 1) {
			emit registerSuccess();
		}
		else {
			emit registerFailure("unknown error!");
		}
	}
	else {
		emit registerFailure("register fail!");
		
	}
	soc_.close();
}

void client::newMsgExec() {
	QString msg = soc_.readAll();

	auto ptr= split(msg);
	for(auto it=ptr->begin();it!=ptr->end();++it) {
		message m(*it);
		switch (m.getInt("type")) {
		case 9:
			msg_exec(m.getInt("sender_id"), m.getString("content"));
			break;
		case 7:
			msg_exec(m);
			break;
		case 3:
			friend_exec(m);
			break;
		default:
			emit errorOccured(m.getString("content"));
		}
	}

}

std::shared_ptr<std::vector<std::string>> client::split(const QString& str) {
	std::shared_ptr<std::vector<std::string>> ptr(new std::vector<std::string>());
	std::string msg = str.toUtf8().toStdString();
	int pos = 0;
	size_t p;
	while ((p = msg.find("\r\n\r\n",pos))!=std::string::npos) {
		ptr->push_back(msg.substr(pos, p-pos));
		pos = p + 4;
	}
	return ptr;
}

void client::friend_request_feedback(int sender_id, bool accept) {
	message m;
	m.add("type", 3);
	m.add("code", accept ? 2 : 3);
	m.add("sender_id", sender_id);
	m.add("recver_id", user_id);
	soc_write(m.getString());
	if (accept) emit FriendRequestAccepted(sender_id);
}

void client::addFriend(const QString& recver_id,const QString& content) {
	message m;
	m.add("type", 3);
	m.add("code", 1);
	m.add("sender_id", user_id);
	m.add("recver_id", recver_id.toInt());
	m.add("content", content);
	soc_write(m.getString());
}

void client::friend_exec(message& m) {
	switch (m.getInt("code")) {
	case 1:
		emit newFriendRequest(m.getInt("sender_id"));
		break;
	case 2:
		emit FriendRequestAccepted(m.getInt("recver_id"));
		break;
	case 3:
		emit FriendRequestRefused(m.getInt("recver_id"));
		break;
	case 4:
		friendListUpdate(m);
		break;
	default:
		emit errorOccured("??");
	}
}

void client::friendListUpdate(message& m) {
	std::vector<std::pair<int, int>> friendList;
	const rapidjson::Value& id = m.getArray("friendID");
	const rapidjson::Value& groupID = m.getArray("friendGroup");
	for(rapidjson::SizeType i=0;i<id.Size();++i) {
		friendList.emplace_back(id[i].GetInt(), groupID[i].GetInt());
	}
	emit FriendListUpdate(friendList);
}

void client::askForFriendList() {
	message m;
	m.add("type", 3);
	m.add("code", 4);
	m.add("sender_id", user_id);
	soc_write(m.getString());
}

void client::askForOfflineMsg() {
	message m;
	m.add("type", 8);
	m.add("sender_id", user_id);
	soc_write(m.getString());
}

void client::pullMsg() {
	message m;
	m.add("type", 7);
	m.add("sender_id", user_id);
	soc_write(m.getString());
}

void client::msg_exec(message& m) {
	bool recv = m.getInt("recver_id") == user_id;
	int id = recv ? m.getInt("sender_id") : m.getInt("recver_id");
	db_.addChatHistory(id, m.getString("content"), recv);
}


void client::msg_exec(int id, std::string content) {
	emit newMsgArrived(id, content);
	db_.addChatHistory(id, content,true);
}

std::shared_ptr<std::vector<std::pair<std::string, bool>>> client::getChatHistory(int id) {
	return db_.getChatHistory(id);
}
