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

public slots:
	void loginSuccess(int id) {
		ui.label_5->setText(QString::number(id));
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
private:
	Ui::LixTalkClass ui;
	std::shared_ptr<client> client_;
};
