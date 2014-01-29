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

plane::plane(QWidget *parent) : QWidget(parent) {

	// we probably wont need these...
//	curly = new Curly();
//	resizer = NULL;
	//
	
	setAttribute(Qt::WA_TranslucentBackground);
	//setAttribute(Qt::NoSystemBackground);
	setWindowFlags(Qt::FramelessWindowHint);
	//setWindowFlags(Qt::WindowStaysOnBottomHint);
	//setWindowFlags(Qt::X11BypassWindowManagerHint);
	setAttribute(Qt::WA_X11DoNotAcceptFocus);
	
	this->corner = 15;

	this->colour = QColor(11, 11, 44, 127);

	//QLabel *mi = new QLabel(this) ;
	//mi->setGeometry(0,0,100,100);
	//QPixmap pix(QString::fromStdString("test.png"));
	//mi->setPixmap(pix);

	get_data();

//mi->show();
 
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
	//QPen pen(Qt::red, 10, Qt::SolidLine);
	QPen pen(Qt::NoPen);
	qp->setPen(pen);

 	QBrush brush;
	brush.setStyle(Qt::SolidPattern); // Fix your problem !
      	brush.setColor(this->colour);
	qp->setBrush(brush);
/*

	QPainterPath p;
	p.lineTo( width-corner, 0);
	p.arcTo(width-(corner*2), 0, corner*2, corner*2, 0, corner*16*3);
	p.lineTo(width, height-corner);
	p.arcTo(width-(corner*2), height-(corner*2),  corner*2, corner*2, 0, -corner*16*3);
	p.lineTo(0+corner, height);
	p.arcTo(0, height-(corner*2),  corner*2, corner*2, 270*16, -corner*16*3);
	p.lineTo(0, 0+corner);
	p.arcTo(0, 0,  corner*2, corner*2, 90*16, corner*16*3);
	p.closeSubpath();

	RenderArea ra(p);
	r.setFilleRule(

*/

qp->drawRoundedRect(0,0, this->width(), this->height(), this->corner, this->corner);


}

void plane::get_data() {

        std::vector<weather_day> items;
        const char *filename = "input";
	//std::string filename = curly->grab_to_file("http://api.worldweatheronline.com/free/v1/weather.ashx?q=DH1+3LE+&format=json&num_of_days=5&key=scrubbed");

        items = parser::parse_file(filename);

	QVBoxLayout *vbox = new QVBoxLayout(this);
	//std::vector<QLabel*> labels;

        for (unsigned i = 1; i < items.size() ; i++) {

		QDate t;

		if (i == 0) {
			t = QDate::currentDate();
		} else {
			t = get_qdate(items[i].date);
		}
		
		//QLabel *tmp_icon = new QLabel;
		//tmp_icon->setPixmap(*get_icon(items[i].url));
		QLabel *tmp_date = new QLabel(get_day(t));
		QLabel *tmp_temp = new QLabel(get_temp(items[i].temp));
		QLabel *tmp_desc = new QLabel(QString::fromStdString(items[i].desc));
		QHBoxLayout *hbox = new QHBoxLayout();

		hbox->addWidget(tmp_date);
		//hbox->addWidget(tmp_icon);
		hbox->addWidget(tmp_temp);
		hbox->addWidget(tmp_desc, Qt::AlignLeft);
		vbox->addLayout(hbox);
        }

	setLayout(vbox);
}

QPixmap * plane::get_icon(std::string url) {
#if 0
	unsigned int pos = url.find_last_of('/');
	std::string path = url.substr(pos+1);

	QFile test(QString::fromStdString(path));

	if (!test.exists()) 
		grab_icon(url);
	
	return new QPixmap(QString::fromStdString(path));
#endif
}

void plane::grab_icon(std::string url) {
#if 0
	if (resizer == NULL) 
		resizer = new Resize();
	
	if (curly == NULL)
		curly = new Curly();

	std::string filename = curly->grab_to_file(url);
	resizer->resize(filename);
#endif
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
		return QString(QChar(0x6708));
		return QString("mon");
		break;

	case 2://6728
		return QString(QChar(0x706b));
		return QString("tue");
		break;

	case 3:
		return QString(QChar(0x6c34));
		return QString("wed");
		break;

	case 4:
		return QString(QChar(0x6728));
		return QString("thu");
		break;

	case 5:
		return QString(QChar(0x91d1));
		return QString("fri");
		break;

	case 6:
		return QString(QChar(0x571f));
		return QString("sat");
		break;

	case 7:
		return QString(QChar(0x65e5));
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

