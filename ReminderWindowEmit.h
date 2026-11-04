#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class ReminderWindowEmit; };
QT_END_NAMESPACE

class ReminderWindowEmit : public QWidget
{
	Q_OBJECT

public:
	ReminderWindowEmit(QWidget *parent = nullptr);
	~ReminderWindowEmit();
	void setData(QString & text);

private slots:
	void onOkButtonClicked();
	

private:
	Ui::ReminderWindowEmit *ui;
	QString reminder_text;
};
