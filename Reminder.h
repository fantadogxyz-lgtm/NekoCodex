#pragma once

#include <QWidget>
#include <QTimer>
#include <QString>
#include <QStringList>
#include "ui_Reminder.h"

class Reminder : public QWidget
{
	Q_OBJECT

public:
	Reminder(QWidget *parent = nullptr);
	~Reminder();
	void setData(int id, const QString& content, QStringList& listOfIntervalsForComboBox, int period);

	int getId();
	QString getContent();
	QStringList getListOfIntervalsForComboBox2();
	int getPeriod();

	void setIsAFK(bool isPeriodZero);
private:
	Ui::Reminder* ui;

	int m_id; 
	QString m_content;
	QStringList m_listOfIntervalsForComboBox; 
	int m_period;
	QTimer* m_timer;

	bool isConnected = false;

	void saveChangeColor();

private slots:
	void onDeleteClicked();
	void onAutoSave();
	void onPeriodChangedReminder();

signals:
	void deleteReminder(int id);
	void contentChangedReminder(int id, QString content);
	void periodChangedReminder(int id, int period);



};
