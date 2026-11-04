#pragma once

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include "TimersForTimerPage.h"
QT_BEGIN_NAMESPACE
namespace Ui { class TimerPage; };
QT_END_NAMESPACE

class TimerPage : public QWidget
{
	Q_OBJECT

public:
	TimerPage(QWidget *parent = nullptr);
	~TimerPage();

private:
	Ui::TimerPage *ui;

	TimersForTimerPage* timersForTimerPage = nullptr;

	int getTimerSecondsFromUI();
	int getSecSecondsFromUI();

	void updateSecDisplay(int seconds);   
	void updateTimerDisplay(int seconds); 

	void makeButtonsDisable();
	void makeButtonsAble();

	void makeSomeButtonsDisableSecondomerWorks();
	void makeSomeButtonsAbleSecondomerOnPause();


private slots:
	void onStartSecClicked();
	void onStopSecClicked();
	void onSbrosSecClicked();

	void onStartTimerClicked();
	void onStopTimerClicked();
	void onSbrosTimerClicked();

	void onTimerSecTickFromTimer(int seconds);
	void onTimerTimerTickFromTimer(int seconds);


	void onHoursPlus();
	void onHoursMinus();

	void onMinutesPlus();
	void onMinutesMinus();

	void onSecondsPlus();
	void onSeconsMinus();

	void onTimeOver();

};
