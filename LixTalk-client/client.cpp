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
	soc_.write(m.getString().c_str());

}

void client::reg_accout(const QString& username, const QString& password) {
	message m;
	m.add("recver_id", 0);
	m.add("type", 1);
	m.add("username", username);
	m.add("password", password);
	soc_.write(m.getString().c_str());
	connect(&soc_, SIGNAL(readyRead()), this, SLOT(registerExec()));
}

void client::send(int recver_id, const QString& msg) {
	message m;
	m.add("type", 9);
	m.add("recver_id", recver_id);
	m.add("sender_id", user_id);
	m.add("message", msg);
	std::string d = m.getString();
	soc_.write(m.getString().c_str());
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

	message m(msg.toUtf8().constData());
	switch (m.getInt("type")) {
	case 9:
		emit newMsgArrived(m.getInt("sender_id"), m.getString("message"));
		break;
	case 3:
		friend_exec(m);
		break;
	default:
		emit errorOccured(m.getString("content"));
	}
}


void client::friend_request_feedback(int sender_id, bool accept) {
	message m;
	m.add("type", 3);
	m.add("code", accept ? 2 : 3);
	m.add("sender_id", sender_id);
	m.add("recver_id", user_id);
	soc_.write(m.getString().c_str());
}

void client::addFriend(const QString& recver_id,const QString& content) {
	message m;
	m.add("type", 3);
	m.add("code", 1);
	m.add("sender_id", user_id);
	m.add("recver_id", recver_id.toInt());
	m.add("content", content);
	soc_.write(m.getString().c_str());
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
	default:
		emit errorOccured("??");
	}
}
