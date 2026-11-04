#pragma once

#include <QWidget>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include<QString>
#include <QMessageBox>
#include "EventChecker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EventCreator; };
QT_END_NAMESPACE

class EventCreator : public QWidget
{
	Q_OBJECT

public:
	EventCreator(QWidget *parent = nullptr);
	~EventCreator();
	void updateTimer();

	void setCreateMode(bool isCreateMode);

	void setData(int id,
		const QString& content,
		const QDateTime& createdAt,
		const QDateTime& remindAt,
		bool isPassed);

private:
	Ui::EventCreator *ui;

	bool m_isCreateMode;

	int m_id;
	QString m_content;
	QDateTime m_createdAt;
	QDateTime m_remindAt;
	bool m_isPassed;

signals:
	void createEvent(const QString& content, const QDateTime& remindAt);
	void updateEvent(int m_id, const  QString& m_content, const  QDateTime& m_remindAt);

private slots:
	void onSaveButtonClicked();
	void onCancelButtonClicked();

};
