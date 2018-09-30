#pragma once
#include <memory>
#include <map>
#include <QtWidgets/QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include "ui_LixTalk.h"
#include "client.h"
#include "MainWindow.h"


class LixTalk : public QMainWindow
{
	Q_OBJECT

public:
	LixTalk(QWidget *parent = Q_NULLPTR);


	void login();
	void register_account();

	//~LixTalk();

public slots:
	void loginSuccess(int id) {
		//ui.label_5->setText(QString::number(id));

		MainWindow* newWin = new MainWindow(std::move(client_),nullptr);
		newWin->show();
		close();
	}
	void loginFailure(std::string msg) {
		//ui.label_5->setText(QString::fromStdString(msg));
	}
	void registerSuccess() {
		//ui.label_5->setText("reg success!");
	}
	void registerFailure(std::string msg) {
		//ui.label_5->setText(QString::fromStdString(msg));
	}


private:
	Ui::LixTalkClass ui;
	std::shared_ptr<client> client_;

};
