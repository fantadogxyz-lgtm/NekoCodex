#pragma once

#include <QObject>
#include <QTimer>
#include <QDebug>
#include "timerendswindow.h"

class TimersForTimerPage  : public QObject
{
	Q_OBJECT

public:
	TimersForTimerPage(TimersForTimerPage &other) = delete;
	void operator=(TimersForTimerPage& other) = delete;
	
	static TimersForTimerPage* instance();
	bool initialize();

private:
	TimersForTimerPage() = default;
	~TimersForTimerPage() = default;

	QTimer* m_secTimer = nullptr;
	QTimer* m_timerTimer = nullptr;

	int secSec = 0;
	int secTimer = 0;



private slots:
	//слоты для тика
	void secTimerTick();
	void secSecTick();

signals:
	//сигналы чтобы передать изменение времени
	void newSecTimer(int seconds);
	void newSecSec(int seconds);
	void timerOver();

public:
	void startSecTimer();
	void stopSecTimer();
	void SbrosSecTimer();

	void startTimerTimer(int seconds);
	void stopTimerTimer();
	void SbrosTimerTimer();


	int getSecSec();
	int getSecTimer();

	bool isTimerWorks();
	bool isSecondomerWorks();


};

