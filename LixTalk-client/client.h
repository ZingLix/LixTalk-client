#ifndef LIXTALK_CLIENT
#define LIXTALK_CLIENT

#include <QtNetwork/QTcpSocket>
#include "DbConnector.h"

class message;

class client:public QObject
{
	Q_OBJECT

public:
	client(const QString& addr, int port);
	client(std::string addr, int port);
	void login(const QString& username, const QString& password);
	void reg_accout(const QString& username, const QString& password);
	void send(int recver_id, const QString& msg);
	void friend_request_feedback(int sender_id, bool accept);
	void addFriend(const QString& recver_id, const QString& content);
	void friend_exec(message& m);
	void friendListUpdate(message& m);
	void askForFriendList();
	void askForOfflineMsg();
	void pullMsg();
	void msg_exec(message& m);
	void msg_exec(int id,std::string content);
	std::shared_ptr<std::vector<std::pair<std::string, bool>>> getChatHistory(int id);
	~client();

public slots:
	void loginExec();
	void registerExec();
	void newMsgExec();

signals:
	void loginSuccess(int id);
	void loginFailure(std::string msg);
	void registerSuccess();
	void registerFailure(std::string msg);
	void newMsgArrived(int sender_id, std::string msg);
	void errorOccured(std::string msg);
	void clientDestroyed();
	void newFriendRequest(int sender_id);
	void FriendRequestAccepted(int recver_id);
	void FriendRequestRefused(int recver_id);
	void FriendListUpdate(const std::vector<std::pair<int, int>>& list);

private:
	std::shared_ptr<std::vector<std::string>> split(const QString& str);
	void soc_write(const std::string & str) {
		soc_.write((str + "\r\n\r\n").c_str());
	}
	QTcpSocket soc_;
	int user_id;
	DbConnector db_;
};

#endif
