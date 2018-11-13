#include "MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(std::shared_ptr<client>&& ptr, QWidget *parent) : QMainWindow(parent),client_(ptr) {
	ui.setupUi(this);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &MainWindow::send);
	connect(ui.pushButton_4, &QPushButton::clicked, this, &MainWindow::addFriend);
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(friendSelectChanged(QTreeWidgetItem*, int)));
	ui.treeWidget->setHeaderLabel("Friend");
	connect(&*client_, SIGNAL(newMsgArrived(int, std::string)), this, SLOT(newMsgExec(int, std::string)));
	connect(&*client_, SIGNAL(errorOccured(std::string)), this, SLOT(errExec(std::string)));
	connect(&*client_, SIGNAL(newFriendRequest(int)), this, SLOT(newFriendRequest(int)));
	connect(&*client_, SIGNAL(FriendRequestAccepted(int)), this, SLOT(FriendRequestAccepted(int)));
	connect(&*client_, SIGNAL(FriendRequestRefused(int)), this, SLOT(FriendRequestRefused(int)));
	connect(&*client_, SIGNAL(clientDestroyed()), this, SLOT(restart()));
	connect(&*client_, SIGNAL(FriendListUpdate(const std::vector<std::pair<int, int>>&)),
		this, SLOT(FriendListInit(const std::vector<std::pair<int, int>>&)));
	client_->askForFriendList();
	client_->askForOfflineMsg();
}


void MainWindow::send() {
	client_->send(curSelectUserID, ui.textEdit_6->toPlainText());
	if (list_map_.find(curSelectUserID) == list_map_.end()) return;
	auto it = ui.tabWidget->widget(list_map_[curSelectUserID])->layout()->itemAt(0);
	QString str = "-> " + ui.textEdit_6->toPlainText();
	qobject_cast<QListWidget *>(it->widget())->addItem(str);
}

void MainWindow::newFriendRequest(int sender_id) {
	QMessageBox msgBox;
	msgBox.setWindowTitle("New Friend Request");
	msgBox.setInformativeText(tr("Receive friend request from ") + QString::number(sender_id) + ".");
	QPushButton *accept = msgBox.addButton(tr("Accept"), QMessageBox::ActionRole);
	QPushButton *refuse = msgBox.addButton(tr("Refuse"), QMessageBox::ActionRole);
	QPushButton *ignore = msgBox.addButton(tr("Ignore"), QMessageBox::ActionRole);
	msgBox.exec();
	if (msgBox.clickedButton() == accept) {
		client_->friend_request_feedback(sender_id, true);
	}
	else if (msgBox.clickedButton() == refuse) {
		client_->friend_request_feedback(sender_id, false);
	}
}

void MainWindow::addFriend() {
	client_->addFriend(ui.textEdit_7->toPlainText(), ui.textEdit_6->toPlainText());
}

void MainWindow::FriendRequestAccepted(int recver_id) {
	if (list_map_.find(recver_id) == list_map_.end()) return;
	auto it = ui.tabWidget->widget(list_map_[recver_id])->layout()->itemAt(0);
	qobject_cast<QListWidget *>(it->widget())->addItem(new QListWidgetItem(QString(QString::number(recver_id) + " accept the friend request.")));
	FriendListAdd(recver_id, 0);
}

void MainWindow::FriendRequestRefused(int recver_id) {
	if (list_map_.find(recver_id) == list_map_.end()) return;

	auto it = ui.tabWidget->widget(list_map_[recver_id])->layout()->itemAt(0);

	qobject_cast<QListWidget *>(it->widget())->addItem(new QListWidgetItem(QString(QString::number(recver_id) + " refuse the friend request.")));
}

void MainWindow::FriendListInit(const std::vector<std::pair<int, int>>& list) {
	for (auto it = list.begin(); it != list.end(); ++it) {
		FriendListAdd(it->first, it->second);
	}
}



void MainWindow::restart() {
	groupMap_.clear();
	userMap_.clear();
	ui.treeWidget->clear();
}

void MainWindow::FriendListAdd(int id, int groupID) {
	if (groupMap_.find(groupID) == groupMap_.end()) {
		groupMap_[groupID] = groupMap_.size();
		ui.treeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList(QString::number(groupID))));
		userMap_.emplace_back();
	}
	userMap_[groupMap_[groupID]].push_back(id);
	ui.treeWidget->topLevelItem(groupMap_[groupID])->addChild(new QTreeWidgetItem(QStringList(QString::number(id))));
}

MainWindow::~MainWindow() {
	disconnect(&*client_, SIGNAL(clientDestroyed()), this, SLOT(restart()));
}

void MainWindow::friendSelectChanged(QTreeWidgetItem* item, int count) {
	auto parent = item->parent();
	if (parent == nullptr) return;
	int i = ui.treeWidget->indexOfTopLevelItem(parent);
	int j = parent->indexOfChild(item);

	ui.label_5->setText(QString::number(userMap_[i][j]));
	curSelectUserID = userMap_[i][j];

	if (list_map_.find(curSelectUserID) == list_map_.end()) {
		QWidget *widget = new QWidget();
		QListWidget *listview = new QListWidget();
		QVBoxLayout *vLayout = new QVBoxLayout();
		vLayout->addWidget(listview);
		widget->setLayout(vLayout);
		ui.tabWidget->addTab(widget, QString::number(curSelectUserID));
		list_map_[curSelectUserID] = ui.tabWidget->indexOf(widget);
		auto ptr = client_->getChatHistory(curSelectUserID);
		for (auto it = ptr->begin(); it != ptr->end(); ++it) {
			QString str;
			if (it->second == true) {
				str += "<- ";
			}
			else {
				str += "-> ";
			}
			str += QString::fromStdString(it->first);
			listview->addItem(new QListWidgetItem(str));
		}
	}

	ui.tabWidget->setCurrentIndex(list_map_[curSelectUserID]);

}