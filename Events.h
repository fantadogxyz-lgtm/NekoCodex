#pragma once

#include<QVector>
#include <QWidget>

#include "oneevent.h"
#include <QGridLayout>
#include "DatabaseEventManager.h"
#include <algorithm>
#include <QEvent>
#include "eventcreator.h"
#include "EventChecker.h"



QT_BEGIN_NAMESPACE
namespace Ui { class Events; };
QT_END_NAMESPACE

class Events : public QWidget
{
	Q_OBJECT

public:
	Events(QWidget *parent = nullptr);
	~Events();

private:
	Ui::Events *ui;

	void setupGrid();
	void loadEvents();
	void cleanEvents();
	void arrangeAllEvents();

	QVector<oneevent*> m_allEvents;

	QGridLayout* m_gridLayout;

	static constexpr int EVENT_WIDTH = 830;
	static constexpr int EVENT_HEIGHT = 80;	
	static constexpr int COLUMNS = 1;

	EventCreator * eventCreator = nullptr;

	bool showAll = true;
	bool showPassed = true;
	bool showComing = true;


private slots:
	void onCreateEventClicked();
	void onCreateEventSignal(const QString& content, const QDateTime& remindAt);
	void onUpdateEventSignal(int id, const QString& content, const QDateTime& remindAt);
	void onShowAllClicked();
	void onShowUpcomingClicked();
	void onShowPassedClicked();
	void onEventDeleted(int id);
	void onUpdateEventFromWidget(int id, const QString& content,
		const QDateTime& createdAt,
		const QDateTime& remindAt,
		bool isPassed);	


public slots:
	void eventPassedSignalFromMenu(int id);
	void eventDeletedSignalFromMenu(int id);


};
