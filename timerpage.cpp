#include "timerpage.h"
#include "ui_timerpage.h"

TimerPage::TimerPage(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::TimerPage())
{
	ui->setupUi(this);

	QPixmap fish(":/res/img/timer/fish-1.png");
	ui->label_4->setPixmap(fish);
	ui->label_4->setScaledContents(true);


	

	connect(ui->pushButtonStartSec, &QPushButton::clicked,
		this, &TimerPage::onStartSecClicked);
	connect(ui->pushButtonSbrosSec, &QPushButton::clicked,
		this, &TimerPage::onSbrosSecClicked);
	connect(ui->pushButtonStopSec, &QPushButton::clicked,
		this, &TimerPage::onStopSecClicked);

	connect(ui->pushButtonStartTimer, &QPushButton::clicked,
		this, &TimerPage::onStartTimerClicked);
	connect(ui->pushButtonSbrosTimer, &QPushButton::clicked,
		this, &TimerPage::onSbrosTimerClicked);
	connect(ui->pushButtonStopTimer, &QPushButton::clicked,
		this, &TimerPage::onStopTimerClicked);

	connect(ui->pushButtonHoursPlus, &QPushButton::clicked,
		this, &TimerPage::onHoursPlus);
	connect(ui->pushButtonHoursMinus, &QPushButton::clicked,
		this, &TimerPage::onHoursMinus);

	connect(ui->pushButtonMinutesPlus, &QPushButton::clicked,
		this, &TimerPage::onMinutesPlus);
	connect(ui->pushButtonMinutesMinus, &QPushButton::clicked,
		this, &TimerPage::onMinutesMinus);

	connect(ui->pushButtonSecondsPlus, &QPushButton::clicked,
		this, &TimerPage::onSecondsPlus);
	connect(ui->pushButtonSecondsMinus, &QPushButton::clicked,
		this, &TimerPage::onSeconsMinus);

	if (!timersForTimerPage) {
		timersForTimerPage = TimersForTimerPage::instance();
		timersForTimerPage->initialize();
	}

	connect(timersForTimerPage, &TimersForTimerPage::newSecSec,
		this, &TimerPage::onTimerSecTickFromTimer);

	connect(timersForTimerPage, &TimersForTimerPage::newSecTimer,
		this, &TimerPage::onTimerTimerTickFromTimer);

	connect(timersForTimerPage, &TimersForTimerPage::timerOver,
		this, &TimerPage::onTimeOver);


	if (timersForTimerPage->isSecondomerWorks()) {
		makeSomeButtonsDisableSecondomerWorks();
		int seconds = timersForTimerPage->getSecSec();
		updateSecDisplay(seconds);
	}
	else {
		makeSomeButtonsAbleSecondomerOnPause();
	}

	if (timersForTimerPage->isTimerWorks()) {
		makeButtonsDisable();
		int seconds = timersForTimerPage->getSecTimer();
		updateTimerDisplay(seconds);
	}
	else {
		makeButtonsAble();
	}
}


TimerPage::~TimerPage()
{
	delete ui;
}

void TimerPage::onStartSecClicked() {
	timersForTimerPage->startSecTimer();
	makeSomeButtonsDisableSecondomerWorks();

	
	
}
void TimerPage::onStopSecClicked() {
	timersForTimerPage->stopSecTimer();
	makeSomeButtonsAbleSecondomerOnPause();

}
void TimerPage::onSbrosSecClicked() {
	timersForTimerPage->SbrosSecTimer();
	makeSomeButtonsAbleSecondomerOnPause();
}



void TimerPage::onStartTimerClicked() {

	int seconds = getTimerSecondsFromUI();

	if (seconds <= 0) {
		qDebug() << "Time must be >0!";
		return;
	}

	if (seconds > 359999) {
		qDebug() << "Max is 99:59:59!";
		return;
	}

	timersForTimerPage->startTimerTimer(seconds);
	makeButtonsDisable();

}
void TimerPage::onStopTimerClicked() {
	makeButtonsAble();
	timersForTimerPage->stopTimerTimer();

}
void TimerPage::onSbrosTimerClicked() {
	makeButtonsAble();
	timersForTimerPage->SbrosTimerTimer();
}



void TimerPage::onTimerSecTickFromTimer(int seconds)
{
	updateSecDisplay(seconds);
}

void TimerPage::onTimerTimerTickFromTimer(int seconds)
{
	updateTimerDisplay(seconds);
}



int TimerPage::getTimerSecondsFromUI()
{

	int hours = ui->labelHoursTimer->text().toInt();
	int minutes = ui->labelMinutesTimer->text().toInt();
	int seconds = ui->labelSecondsTimer->text().toInt();


	return hours * 3600 + minutes * 60 + seconds;
}

int TimerPage::getSecSecondsFromUI()
{
	int hours = ui->labelHoursSecondomer->text().toInt();
	int minutes = ui->labelMinutesSecondomer->text().toInt();
	int seconds = ui->labelSecondsSecondomer->text().toInt();


	return hours * 3600 + minutes * 60 + seconds;
}

void TimerPage::updateSecDisplay(int seconds)
{
	int hours = seconds / 3600;
	int minutes = (seconds % 3600) / 60;
	int secs = seconds % 60;

	ui->labelHoursSecondomer->setText(QString("%1").arg(hours, 2, 10, QChar('0')));
	ui->labelMinutesSecondomer->setText(QString("%1").arg(minutes, 2, 10, QChar('0')));
	ui->labelSecondsSecondomer->setText(QString("%1").arg(secs, 2, 10, QChar('0')));
}

void TimerPage::updateTimerDisplay(int seconds)
{
	int hours = seconds / 3600;
	int minutes = (seconds % 3600) / 60;
	int secs = seconds % 60;

	ui->labelHoursTimer->setText(QString("%1").arg(hours, 2, 10, QChar('0')));
	ui->labelMinutesTimer->setText(QString("%1").arg(minutes, 2, 10, QChar('0')));
	ui->labelSecondsTimer->setText(QString("%1").arg(secs, 2, 10, QChar('0')));
}

void TimerPage::makeButtonsDisable()
{
	ui->pushButtonHoursMinus->setDisabled(true);
	ui->pushButtonHoursPlus->setDisabled(true);
	ui->pushButtonMinutesMinus->setDisabled(true);
	ui->pushButtonMinutesPlus->setDisabled(true);
	ui->pushButtonSecondsMinus->setDisabled(true);
	ui->pushButtonSecondsPlus->setDisabled(true);

	ui->pushButtonHoursMinus->setStyleSheet("background-color: rgb(255, 255, 255); border - radius: 5px; color: rgb(115, 115, 172); ");
	ui->pushButtonHoursPlus->setStyleSheet("background-color: rgb(255, 255, 255); border - radius: 5px; color: rgb(115, 115, 172); ");
	ui->pushButtonMinutesMinus->setStyleSheet("background-color: rgb(255, 255, 255); border - radius: 5px; color: rgb(115, 115, 172); ");
	ui->pushButtonMinutesPlus->setStyleSheet("background-color: rgb(255, 255, 255); border - radius: 5px; color: rgb(115, 115, 172); ");
	ui->pushButtonSecondsMinus->setStyleSheet("background-color: rgb(255, 255, 255); border - radius: 5px; color: rgb(115, 115, 172); ");
	ui->pushButtonSecondsPlus->setStyleSheet("background-color: rgb(255, 255, 255); border - radius: 5px; color: rgb(115, 115, 172); ");

	ui->pushButtonStartTimer->setDisabled(true);
	ui->pushButtonSbrosTimer->setDisabled(false);
	ui->pushButtonStopTimer->setDisabled(false);


	ui->pushButtonStartTimer->setStyleSheet("background-color: rgb(186, 181, 214); color: rgb(78, 75, 89); ");
	ui->pushButtonSbrosTimer->setStyleSheet("background-color: rgb(222, 216, 255); color: rgb(115, 115, 172); ");
	ui->pushButtonStopTimer->setStyleSheet("background-color: rgb(222, 216, 255); color: rgb(115, 115, 172); ");
}

void TimerPage::makeButtonsAble()
{
	ui->pushButtonHoursMinus->setDisabled(false);
	ui->pushButtonHoursPlus->setDisabled(false);
	ui->pushButtonMinutesMinus->setDisabled(false);
	ui->pushButtonMinutesPlus->setDisabled(false);
	ui->pushButtonSecondsMinus->setDisabled(false);
	ui->pushButtonSecondsPlus->setDisabled(false);

	ui->pushButtonHoursMinus->setStyleSheet("background-color: rgb(115, 115, 172); border - radius: 5px; color:rgb(255, 255, 255); ");
	ui->pushButtonHoursPlus->setStyleSheet("background-color: rgb(115, 115, 172); border - radius: 5px; color:rgb(255, 255, 255); ");
	ui->pushButtonMinutesMinus->setStyleSheet("background-color: rgb(115, 115, 172); border - radius: 5px; color:rgb(255, 255, 255);  ");
	ui->pushButtonMinutesPlus->setStyleSheet("background-color: rgb(115, 115, 172); border - radius: 5px; color:rgb(255, 255, 255);  ");
	ui->pushButtonSecondsMinus->setStyleSheet("background-color: rgb(115, 115, 172); border - radius: 5px; color:rgb(255, 255, 255);  ");
	ui->pushButtonSecondsPlus->setStyleSheet("background-color: rgb(115, 115, 172); border - radius: 5px; color:rgb(255, 255, 255); ");

	ui->pushButtonStartTimer->setDisabled(false);
	ui->pushButtonSbrosTimer->setDisabled(false);
	ui->pushButtonStopTimer->setDisabled(true);


	ui->pushButtonStartTimer->setStyleSheet("background-color: rgb(222, 216, 255); color: rgb(115, 115, 172); ");
	ui->pushButtonSbrosTimer->setStyleSheet("background-color: rgb(222, 216, 255); color: rgb(115, 115, 172); ");
	ui->pushButtonStopTimer->setStyleSheet("background-color: rgb(186, 181, 214); color: rgb(78, 75, 89); ");
}

void TimerPage::makeSomeButtonsDisableSecondomerWorks()
{
	ui->pushButtonStartSec->setDisabled(true);
	ui->pushButtonSbrosSec->setDisabled(false);
	ui->pushButtonStopSec->setDisabled(false);

	ui->pushButtonStartSec->setStyleSheet("background-color: rgb(186, 181, 214); color: rgb(78, 75, 89); ");
	ui->pushButtonSbrosSec->setStyleSheet("background-color: rgb(222, 216, 255); color: rgb(115, 115, 172); ");
	ui->pushButtonStopSec->setStyleSheet("background-color: rgb(222, 216, 255); color: rgb(115, 115, 172); ");
}

void TimerPage::makeSomeButtonsAbleSecondomerOnPause()
{
	ui->pushButtonStartSec->setDisabled(false);
	ui->pushButtonSbrosSec->setDisabled(false);
	ui->pushButtonStopSec->setDisabled(true);

	ui->pushButtonStartSec->setStyleSheet("background-color: rgb(222, 216, 255); color: rgb(115, 115, 172); ");
	ui->pushButtonSbrosSec->setStyleSheet("background-color: rgb(222, 216, 255); color: rgb(115, 115, 172); ");
	ui->pushButtonStopSec->setStyleSheet("background-color: rgb(186, 181, 214); color: rgb(78, 75, 89); ");
}


void TimerPage::onHoursPlus()
{
	int hours = ui->labelHoursTimer->text().toInt();
	hours++;
	if (hours > 99) hours = 99;  // Максимум 99 часов
	ui->labelHoursTimer->setText(QString("%1").arg(hours, 2, 10, QChar('0')));
}

void TimerPage::onHoursMinus()
{
	int hours = ui->labelHoursTimer->text().toInt();
	hours--;
	if (hours < 0) hours = 0;  // Минимум 0
	ui->labelHoursTimer->setText(QString("%1").arg(hours, 2, 10, QChar('0')));
}

void TimerPage::onMinutesPlus()
{
	int minutes = ui->labelMinutesTimer->text().toInt();
	minutes++;
	if (minutes > 59) minutes = 59;  
	ui->labelMinutesTimer->setText(QString("%1").arg(minutes, 2, 10, QChar('0')));
}

void TimerPage::onMinutesMinus()
{
	int minutes = ui->labelMinutesTimer->text().toInt();
	minutes--;
	if (minutes < 0) minutes = 0;
	ui->labelMinutesTimer->setText(QString("%1").arg(minutes, 2, 10, QChar('0')));
}

void TimerPage::onSecondsPlus()
{
	int seconds = ui->labelSecondsTimer->text().toInt();
	seconds++;
	if (seconds > 59) seconds = 59;
	ui->labelSecondsTimer->setText(QString("%1").arg(seconds, 2, 10, QChar('0')));
}

void TimerPage::onSeconsMinus()
{
	int seconds = ui->labelSecondsTimer->text().toInt();
	seconds--;
	if (seconds < 0) seconds = 0;
	ui->labelSecondsTimer->setText(QString("%1").arg(seconds, 2, 10, QChar('0')));
}

void TimerPage::onTimeOver()
{
	makeButtonsAble();
}


