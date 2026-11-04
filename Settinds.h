#pragma once

#include <QWidget>
#include "Neko_settings.h"
#include <QStringList>
#include <QGridLayout>
#include <QVector>
#include "DatabaseReminderManager.h"
#include <algorithm>
#include <QEvent>
#include "Reminder.h"
#include "ReminderTimersManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Settinds; };
QT_END_NAMESPACE

class Settinds : public QWidget
{
	Q_OBJECT

public:
	Settinds(QWidget *parent = nullptr);
	~Settinds();

	void loadReminders();


private:
	Ui::Settinds *ui;
	void setSettings();
	void makeSettingsRandomMessagesDisable(bool isAllowed);
	void fillIntervalsComboBox();

	QStringList listOfIntervalsForComboBox = {"5", "10", "15", "30", "45", "60", "90", "120"};
	QStringList listOfIntervalsForComboBox2 = { "0", "5", "10", "15", "30", "45", "60", "90", "120"};

	void arrangeReminders();
	void setupGrid();
	void cleanReminders();

	QVector<Reminder*> m_reminders;

	QGridLayout* m_gridLayout;

	static constexpr int REMINDER_WIDTH = 480;
	static constexpr int REMINDER_HEIGHT = 190;
	static constexpr int COLUMNS = 1;
	bool m_firstShow;
	

private slots:
	void onCheckBoxIsPositionFixedClicked();
	void onCheckBoxIsActivityLow();
	void onCheckBoxIsRandomMessagesAllowed();

	void onReminderDeleted(int id);
	void onReminderContentChanged(int id,const QString& content);
	void onReminderPeriodChanged(int id, int period);
	void onCreateReminderClicked();

signals:

	void signalIsActivityLow(bool isActivityLowNow);
	void signalIsPositionFixed(bool isPositionFixedNow);

protected:
	void showEvent(QShowEvent* event) override;

};
