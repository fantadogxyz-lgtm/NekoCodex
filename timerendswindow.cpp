#include "timerendswindow.h"

TimerEndsWindow::TimerEndsWindow(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::TimerEndsWindowClass())
{
	ui->setupUi(this);

	connect(ui->pushButton, &QPushButton::clicked,
		this, &TimerEndsWindow::onCloseWindowPressed);

	QPixmap fish(":/res/img/timer/fish-2.png");
	ui->label_3->setPixmap(fish);
	ui->label_3->setScaledContents(true);
}

TimerEndsWindow::~TimerEndsWindow()
{
	delete ui;
}

void TimerEndsWindow::onCloseWindowPressed() {
	this->close();
}

