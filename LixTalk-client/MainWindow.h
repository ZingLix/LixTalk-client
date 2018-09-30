#pragma once
#include <QtWidgets/QMainWindow>
#include <memory>
#include <map>
#include <QtWidgets/QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include "ui_MainWindow.h"
#include "client.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(std::shared_ptr<client>&& ptr,QWidget *parent = Q_NULLPTR);
	void send();
	void addFriend();

	~MainWindow();

public slots:
	void newMsgExec(int sender_id, std::string msg) {
		std::string newMsg = "<- " + msg;
		if (list_map_.find(sender_id) == list_map_.end()) return;
		auto it = ui.tabWidget->widget(list_map_[sender_id])->layout()->itemAt(0);
		qobject_cast<QListWidget *>(it->widget())->addItem(new QListWidgetItem(QString::fromStdString(newMsg)));
	}
	void errExec(std::string msg) {
		ui.label_5->setText(QString::fromStdString(msg));
	}
	void newClient() {
		disconnect(ui.pushButton_3, &QPushButton::clicked, this, &MainWindow::send);
	}
	void newFriendRequest(int sender_id);
	void FriendRequestAccepted(int recver_id);
	void FriendRequestRefused(int recver_id);
	void FriendListInit(const std::vector<std::pair<int, int>>& list);
	void restart();
	void FriendListAdd(int id, int groupID);
	void friendSelectChanged(QTreeWidgetItem*, int);

private:
	Ui::MainWindow ui;
	std::shared_ptr<client> client_;
	std::map<int, int> groupMap_;
	std::vector<std::vector<int>> userMap_;
	int curSelectUserID;
	std::map<int, int> list_map_;
};