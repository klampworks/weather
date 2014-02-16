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
#include <QTimer>
#include <cassert>

plane::plane(std::string key_p, std::string postcode_p, 
	QWidget *parent) 
	: QWidget(parent) 
{
	key = key_p;
	postcode = postcode_p;
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_X11DoNotAcceptFocus);
	
	vbox = new QVBoxLayout(this);
	//hbox = new QHBoxLayout();

	days = 5;

	/* Important to keep days inline (mon, tue, wed etc.) */
	QFont font("Monospace");

	for (int i = 0; i < days; i++) {

		tmp_icon.push_back(new QLabel);
		tmp_date.push_back(new QLabel);
		tmp_temp.push_back(new QLabel);
		tmp_desc.push_back(new QLabel);

		/* We want this to be fixed width. */
		tmp_date[i]->setFont(font);

		hbox.push_back(new QHBoxLayout);
		hbox[i]->addWidget(tmp_date[i]);
		hbox[i]->addWidget(tmp_icon[i]);
		hbox[i]->addWidget(tmp_temp[i]);
		hbox[i]->addWidget(tmp_desc[i], Qt::AlignLeft);
		vbox->addLayout(hbox[i]);
	}

		/*
	tmp_icon = new QLabel;
	tmp_date = new QLabel;
	tmp_temp = new QLabel;
	tmp_desc = new QLabel;

	QFont font("Monospace");
	tmp_date->setFont(font);
	hbox->addWidget(tmp_date);
	hbox->addWidget(tmp_icon);
	hbox->addWidget(tmp_temp);
	hbox->addWidget(tmp_desc, Qt::AlignLeft);
	*/

	setLayout(vbox);

	this->corner = 15;
	this->colour = QColor(11, 11, 44, 127);

	get_data();

	tmr = new QTimer;
	connect(tmr, SIGNAL(timeout()), this, SLOT(get_data()));
	tmr->start(1000);
}

void plane::paintEvent(QPaintEvent *e) 
{
	Q_UNUSED(e);
	QPainter qp(this);
	drawLines(&qp);
	QDesktopWidget *desktop = QApplication::desktop();
	int width = desktop->width();
	move((width - this->width())-1, 410);
	
}

void plane::drawLines(QPainter *qp) 
{
	qp->setOpacity(1.0);
	QPen pen(Qt::NoPen);
	qp->setPen(pen);

 	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
      	brush.setColor(this->colour);
	qp->setBrush(brush);

	qp->drawRoundedRect(0,0, this->width(), this->height(), 
		this->corner, this->corner);

}

void plane::get_data() {

std::cout << "sldfl;" << std::endl;
        std::vector<weather_day> items;
	std::string filename = grab.grab_to_file(
		"http://api.worldweatheronline.com/free/v1/weather.ashx"
		"?q=" + postcode + "&format=json&num_of_days=5&key=" + key);

        items = parser::parse_file(filename.c_str());


	/* +1 because we are skipping one. The current day is listed twice. */
	assert(days + 1== items.size());
        for (int i = 0; i < days; i++) {

		QDate t;

		if (items[i+1].date.empty())
			continue;
		else
			t = get_qdate(items[i+1].date);
		
		tmp_icon[i]->setPixmap(*get_icon(items[i+1].url));
		tmp_date[i]->setText(get_day(t));
		tmp_temp[i]->setText(get_temp(items[i+1].temp));
		tmp_desc[i]->setText(QString::fromStdString(items[i+1].desc));

		//vbox->addLayout(hbox);
        }

	//setLayout(vbox);
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
