#include "LixTalk.h"
#include <QMessageBox>

LixTalk::LixTalk(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	QObject::connect(ui.pushButton, &QPushButton::clicked, this, &LixTalk::login);
	QObject::connect(ui.pushButton_2, &QPushButton::clicked, this, &LixTalk::register_account);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &LixTalk::send);
	connect(ui.pushButton_4, &QPushButton::clicked, this, &LixTalk::addFriend);
	ui.treeWidget->setHeaderLabel("Friend");
}

void LixTalk::login() {
	client_ = std::make_shared<client>(ui.textEdit->toPlainText(), ui.textEdit_2->toPlainText().toInt());
	client_->login(ui.textEdit_3->toPlainText(), ui.textEdit_4->toPlainText());
	connect(&*client_, SIGNAL(loginSuccess(int)), this, SLOT(loginSuccess(int)));
	connect(&*client_, SIGNAL(loginFailure(std::string)), this, SLOT(loginFailure(std::string)));
	//connect(&*client_, SIGNAL(clientDestroyed()), this, SLOT(newClient()));
}

void LixTalk::register_account() {
	client_ = std::make_shared<client>(ui.textEdit->toPlainText(), ui.textEdit_2->toPlainText().toInt());
	client_->reg_accout(ui.textEdit_3->toPlainText(), ui.textEdit_4->toPlainText());
	connect(&*client_, SIGNAL(registerSuccess()), this, SLOT(registerSuccess()));
	connect(&*client_, SIGNAL(registerFailure(std::string)), this, SLOT(registerFailure(std::string)));
}

void LixTalk::send() {
	client_->send(ui.textEdit_5->toPlainText().toInt(), ui.textEdit_6->toPlainText());

}

void LixTalk::newFriendRequest(int sender_id) {
	QMessageBox msgBox;
	msgBox.setWindowTitle("New Friend Request");
	msgBox.setInformativeText(tr("Receive friend request from ")+QString::number(sender_id)+".");
	QPushButton *accept = msgBox.addButton(tr("Accept"), QMessageBox::ActionRole);
	QPushButton *refuse = msgBox.addButton(tr("Refuse"), QMessageBox::ActionRole);
	QPushButton *ignore = msgBox.addButton(tr("Ignore"), QMessageBox::ActionRole);
	msgBox.exec();
	if(msgBox.clickedButton()==accept) {
		client_->friend_request_feedback(sender_id,true);
	}else if(msgBox.clickedButton()==refuse) {
		client_->friend_request_feedback(sender_id, false);
	}
}

void LixTalk::addFriend() {
	client_->addFriend(ui.textEdit_7->toPlainText(),ui.textEdit_6->toPlainText());
}

void LixTalk::FriendRequestAccepted(int recver_id) {
	ui.listWidget->addItem(new QListWidgetItem(QString(QString::number( recver_id )+ " accept the friend request.")));
	FriendListAdd(recver_id, 0);
}

void LixTalk::FriendRequestRefused(int recver_id) {
	ui.listWidget->addItem(new QListWidgetItem(QString(QString::number(recver_id) + " refuse the friend request.")));
}

void LixTalk::FriendListInit(const std::vector<std::pair<int, int>>& list) {
	for(auto it=list.begin();it!=list.end();++it) {
		FriendListAdd(it->first, it->second);
	}
}



void LixTalk::restart() {
	groupMap_.clear();
	userMap_.clear();
	ui.treeWidget->clear();
}

void LixTalk::FriendListAdd(int id, int groupID) {
	if (groupMap_.find(groupID) == groupMap_.end()) {
		groupMap_[groupID] = new QTreeWidgetItem(QStringList(QString::number(groupID)));
		ui.treeWidget->addTopLevelItem(groupMap_[groupID]);
	}
	userMap_.insert(std::make_pair(id, std::make_pair(groupID, new QTreeWidgetItem(QStringList(QString::number(id))))));
	groupMap_[groupID]->addChild(userMap_[id].second);
}

LixTalk::~LixTalk() {
	disconnect(&*client_, SIGNAL(clientDestroyed()), this, SLOT(restart()));
}
