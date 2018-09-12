#include "LixTalk.h"

LixTalk::LixTalk(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	QObject::connect(ui.pushButton, &QPushButton::clicked, this, &LixTalk::login);
	QObject::connect(ui.pushButton_2, &QPushButton::clicked, this, &LixTalk::register_account);
}

void LixTalk::login() {
	client_ = std::make_shared<client>(ui.textEdit->toPlainText(), ui.textEdit_2->toPlainText().toInt());
	client_->login(ui.textEdit_3->toPlainText(), ui.textEdit_4->toPlainText());
	connect(&*client_, SIGNAL(loginSuccess(int)), this, SLOT(loginSuccess(int)));
	connect(&*client_, SIGNAL(loginFailure(std::string)), this, SLOT(loginFailure(std::string)));
}

void LixTalk::register_account() {
	client_ = std::make_shared<client>(ui.textEdit->toPlainText(), ui.textEdit_2->toPlainText().toInt());
	client_->reg_accout(ui.textEdit_3->toPlainText(), ui.textEdit_4->toPlainText());
	connect(&*client_, SIGNAL(registerSuccess()), this, SLOT(registerSuccess()));
	connect(&*client_, SIGNAL(registerFailure(std::string)), this, SLOT(registerFailure(std::string)));
}
