#include <QApplication>
#include "plane.hpp"

int main(int argc, char **argv) 
{
	QApplication app(argc, argv);
	plane window;
	window.show();
	return app.exec();
}
