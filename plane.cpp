#include <QPainter>
#include <iostream>
#include <QFile>
#include <QDate>
#include <QLabel>

#include "weather_day.hpp"
#include "parser.hpp"
#include <vector>
#include <QVBoxLayout>
#include <sstream>
#include <QDesktopWidget>
#include "plane.hpp"
#include <QApplication>

plane::plane(std::string key_p, std::string postcode_p, QWidget *parent) : QWidget(parent) {

	key = key_p;
	postcode = postcode_p;
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_X11DoNotAcceptFocus);
	
	this->corner = 15;
	this->colour = QColor(11, 11, 44, 127);
	get_data();
}

void plane::paintEvent(QPaintEvent *e) {

	Q_UNUSED(e);
	QPainter qp(this);
	drawLines(&qp);
	QDesktopWidget *desktop = QApplication::desktop();
	int width = desktop->width();
	move((width - this->width())-1, 410);
	
}

void plane::drawLines(QPainter *qp) {

	qp->setOpacity(1.0);
	QPen pen(Qt::NoPen);
	qp->setPen(pen);

 	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
      	brush.setColor(this->colour);
	qp->setBrush(brush);

qp->drawRoundedRect(0,0, this->width(), this->height(), this->corner, this->corner);


}

void plane::get_data() {

        std::vector<weather_day> items;
	std::string filename = grab.grab_to_file(
		"http://api.worldweatheronline.com/free/v1/weather.ashx"
		"?q=" + postcode + "&format=json&num_of_days=5&key=" + key);

        items = parser::parse_file(filename.c_str());

	QVBoxLayout *vbox = new QVBoxLayout(this);

        for (const auto &item: items) {

		QDate t;

		if (item.date.empty())
			continue;
		else
			t = get_qdate(item.date);
		
		QLabel *tmp_icon = new QLabel;
		tmp_icon->setPixmap(*get_icon(item.url));
		QLabel *tmp_date = new QLabel(get_day(t));
		QLabel *tmp_temp = new QLabel(get_temp(item.temp));
		QLabel *tmp_desc = new QLabel(QString::fromStdString(item.desc));
		QHBoxLayout *hbox = new QHBoxLayout();

		QFont font("Monospace");
		tmp_date->setFont(font);
		hbox->addWidget(tmp_date);
		hbox->addWidget(tmp_icon);
		hbox->addWidget(tmp_temp);
		hbox->addWidget(tmp_desc, Qt::AlignLeft);
		vbox->addLayout(hbox);
        }

	setLayout(vbox);
}

QPixmap * plane::get_icon(std::string url) {
	unsigned int pos = url.find_last_of('/');
	std::string path = url.substr(pos+1);

	QFile test(QString::fromStdString(path));

	if (!test.exists()) 
		grab_icon(url);
	
	return new QPixmap(QString::fromStdString(path));
}

void plane::grab_icon(std::string url) {

	std::string filename = grab.grab_to_file(url);

	QImage px(QString::fromStdString(filename));

	QImage px_s = px.scaledToWidth(16, Qt::SmoothTransformation);
	px_s.save(QString::fromStdString(filename));
}


QDate plane::get_qdate(std::string date) {

        std::stringstream ss(date);
	std::vector<int> parts;
        std::string tmp;
	std::istringstream iss;

	while(std::getline(ss, tmp, '-')) {

		std::cout << "tmp = " << tmp << std::endl;

		iss.str(tmp);
		int tmp_int=0;
		iss >> tmp_int;
		parts.push_back(tmp_int);
		iss.seekg(0);

        }

 
	return QDate(parts[0], parts[1], parts[2]);
}

QString plane::get_day(QDate date) {

	int day = date.dayOfWeek();

	switch(day) {
	
	case 1:
		//return QString(QChar(0x6708));
		return QString("mon");
		break;

	case 2://6728
		//return QString(QChar(0x706b));
		return QString("tue");
		break;

	case 3:
		//return QString(QChar(0x6c34));
		return QString("wed");
		break;

	case 4:
		//return QString(QChar(0x6728));
		return QString("thu");
		break;

	case 5:
		//return QString(QChar(0x91d1));
		return QString("fri");
		break;

	case 6:
		//return QString(QChar(0x571f));
		return QString("sat");
		break;

	case 7:
		//return QString(QChar(0x65e5));
		return QString("sun");
		break;


	}
}

QString plane::get_temp(std::string temp) {
	if (temp.length() == 1) {

		temp = " " + temp;

	}

	return QString::fromStdString(temp) + QString(QChar(0xb0));
}

void plane::resize(const std::string &filename) {

	QImage px(QString::fromStdString(filename));
	QImage px_s = px.scaledToWidth(16, Qt::SmoothTransformation);
	px_s.save(QString::fromStdString(filename));
}
