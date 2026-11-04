#pragma once

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QVector>
#include "DatabaseEventManager.h"
#include "eventpassed.h"

class EventChecker : public QObject
{
	Q_OBJECT

public:


	EventChecker(EventChecker& other) = delete;
	void operator=(EventChecker other) = delete;

	static  EventChecker* instance();
	bool initialize();
	void updateTimer();

private:
	EventChecker() = default;
	~EventChecker() = default;

	QTimer* m_timer = nullptr;

	QVector<int> m_notifiedEvents;

signals:
	void eventPassed(int id);
	void eventDeleted(int id);

private slots:
	void checkEvents();
	void deleteEvent(int id);
};
