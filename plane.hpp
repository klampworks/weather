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

class QTimer;
#include <QLabel>
class plane : public QWidget { 

	Q_OBJECT
	public:
	plane(std::string, std::string, QWidget *parent = NULL);
	void paintEvent(QPaintEvent *e);
	void drawLines(QPainter *qp);
	QPixmap * get_icon(std::string url);
	void grab_icon(std::string url);
	QDate get_qdate(std::string date);
	QString get_day(QDate date);
	QString get_temp(std::string temp);
	void resize(const std::string&);
	QTimer *tmr;

	int corner;
	QColor colour;
	grabber grab;
	std::string key;
	std::string postcode;

	int days;
	unsigned tmr_interval;

	QVBoxLayout *vbox;
	std::vector<QHBoxLayout*> hbox;
	std::vector<QLabel*> tmp_icon;
	std::vector<QLabel*> tmp_date;
	std::vector<QLabel*> tmp_temp;
	std::vector<QLabel*> tmp_desc;
	protected slots:
		void get_data();
};
#endif
