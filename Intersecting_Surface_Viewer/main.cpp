#include "Win.h"
#include <QApplication>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Win window;
	window.show();

    return app.exec();
}