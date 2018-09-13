#ifndef LIXTALK_CLIENT
#define LIXTALK_CLIENT

#include <QtNetwork/QTcpSocket>

class client:public QObject
{
	Q_OBJECT

public:
	client(const QString& addr, int port);
	client(std::string addr, int port);
	void login(const QString& username, const QString& password);
	void reg_accout(const QString& username, const QString& password);
	void send(int recver_id, const QString& msg);
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

private:
	QTcpSocket soc_;
	int user_id;
};

#endif
