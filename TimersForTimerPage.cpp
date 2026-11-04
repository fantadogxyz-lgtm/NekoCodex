#include "TimersForTimerPage.h"

TimersForTimerPage* TimersForTimerPage::instance() {
	static TimersForTimerPage m_instance;
	return &m_instance;
}

bool TimersForTimerPage::initialize() {
	if (!m_secTimer) {
		m_secTimer = new QTimer(this);
		m_secTimer->setInterval(1000);
		connect(m_secTimer, &QTimer::timeout, this, &TimersForTimerPage::secSecTick);
	}

	if (!m_timerTimer) {
		m_timerTimer = new QTimer(this);
		m_timerTimer->setInterval(1000);
		connect(m_timerTimer, &QTimer::timeout, this, &TimersForTimerPage::secTimerTick);
	}

	return true;
}

void TimersForTimerPage::secSecTick() {
	if (secSec >= 359999) {
		stopSecTimer();
		qDebug() << "Max sec time";
		emit newSecSec(secSec);
		return;
	}
	secSec++;
	emit newSecSec(secSec);
}


void TimersForTimerPage::secTimerTick() {
	if (secTimer <= 0) {
		stopTimerTimer();
		qDebug() << "Time over!";
		emit timerOver();

		auto timerEndsWindow = new TimerEndsWindow();
		timerEndsWindow->setAttribute(Qt::WA_DeleteOnClose);
		timerEndsWindow->setAttribute(Qt::WA_QuitOnClose, false);
		timerEndsWindow->show();
		
		return;
	}
	secTimer--;
	emit newSecTimer(secTimer);
}


void TimersForTimerPage::startSecTimer() 
{
	if (!m_secTimer->isActive()) {
		m_secTimer->start();
	}
	
}

void TimersForTimerPage::stopSecTimer() {
	if (m_secTimer->isActive()) {
		m_secTimer->stop();
	}
}
void TimersForTimerPage::SbrosSecTimer() {
	stopSecTimer();
	secSec = 0;
	emit newSecSec(0);
}




void TimersForTimerPage::startTimerTimer(int seconds)
{
	if (!m_timerTimer->isActive()) {
		secTimer = seconds;
		emit newSecTimer(secTimer);
		m_timerTimer->start();
		
	}

}

void TimersForTimerPage::stopTimerTimer()
{
	if (m_timerTimer->isActive()) {
		m_timerTimer->stop();
	}
}

void TimersForTimerPage::SbrosTimerTimer()
{
	stopTimerTimer();
	secTimer = 0;
	emit newSecTimer(0);
}

int TimersForTimerPage::getSecSec()
{
	return secSec;
}

int TimersForTimerPage::getSecTimer()
{
	return secTimer;
}

bool TimersForTimerPage::isTimerWorks()
{
	return m_timerTimer->isActive();
}

bool TimersForTimerPage::isSecondomerWorks()
{
	return m_secTimer->isActive();
}






