#include "LixTalk.h"
#include <QtWidgets/QApplication>
#include <chrono>
#include <iostream>




int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LixTalk w;
	w.show();
	return a.exec();
}
