#include <QApplication>
#include "plane.hpp"
#include <cassert>

int main(int argc, char **argv) 
{
	assert(argc == 3);
	QApplication app(argc, argv);
	plane window(argv[1], argv[2]);
	window.show();
	return app.exec();
}
