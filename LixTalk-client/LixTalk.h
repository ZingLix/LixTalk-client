#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LixTalk.h"
#include "client.h"

class LixTalk : public QMainWindow
{
	Q_OBJECT

public:
	LixTalk(QWidget *parent = Q_NULLPTR);


	void login();
	void register_account();
	void send();

public slots:
	void loginSuccess(int id) {
		ui.label_5->setText(QString::number(id));
		connect(&*client_, SIGNAL(newMsgArrived(int, std::string)), this, SLOT(newMsgExec(int, std::string)));
		connect(ui.pushButton_3, &QPushButton::clicked, this, &LixTalk::send);
		connect(&*client_, SIGNAL(errorOccured(std::string)), this, SLOT(errExec(std::string)));
	}
	void loginFailure(std::string msg) {
		ui.label_5->setText(QString::fromStdString(msg));
	}
	void registerSuccess() {
		ui.label_5->setText("reg success!");
	}
	void registerFailure(std::string msg) {
		ui.label_5->setText(QString::fromStdString(msg));
	}
	void newMsgExec(int sender_id,std::string msg) {
		std::string newMsg = std::to_string(sender_id)+ ":" + msg;
		ui.listWidget->addItem(new QListWidgetItem(QString::fromStdString(newMsg)));
	}
	void errExec(std::string msg) {
		ui.label_5->setText(QString::fromStdString(msg));
	}
	void newClient() {
		disconnect(ui.pushButton_3, &QPushButton::clicked, this, &LixTalk::send);
	}
private:
	Ui::LixTalkClass ui;
	std::shared_ptr<client> client_;
};
