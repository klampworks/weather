#ifndef PLANE_HPP
#define PLANE_HPP
#include <QPainter>
#include <iostream>
#include <QFile>

//#include "weather_day.hpp"
//#include "parser.hpp"
#include <vector>
#include <QVBoxLayout>
#include <sstream>
#include <QDesktopWidget>

#include "grabber.hpp"

class plane : public QWidget { 

	public:
	plane(std::string, std::string, QWidget *parent = NULL);
	void paintEvent(QPaintEvent *e);
	void drawLines(QPainter *qp);
	void get_data();
	QPixmap * get_icon(std::string url);
	void grab_icon(std::string url);
	QDate get_qdate(std::string date);
	QString get_day(QDate date);
	QString get_temp(std::string temp);
	void resize(const std::string&);

	int corner;
	QColor colour;
	grabber grab;
	std::string key;
	std::string postcode;
};
#endif
