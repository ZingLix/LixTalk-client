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

void client::loginExec() {
	QString msg = soc_.readAll();
	message m(msg.toUtf8().constData());
	if(m.getInt("type")==0) {
		if(m.getInt("result")==1) {
			emit loginSuccess(m.getInt("recver_id"));
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
