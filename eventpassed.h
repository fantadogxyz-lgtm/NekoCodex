#pragma once

#include <QWidget>
#include "ui_eventpassed.h"
#include <QDateTime>

class EventPassed : public QWidget
{
	Q_OBJECT

public:
	EventPassed(QWidget *parent = nullptr);
	~EventPassed();
	void setData(int id, QString content, QDateTime remindAt);

private:
	Ui::EventPassed* ui;
	int m_id;
	QDateTime m_remindAt;
	QString m_content;

signals:
	void eventToDelete(int id);

private slots:
	void onOkButtonClicked();
	void onDeleteButtonClicked();
};
