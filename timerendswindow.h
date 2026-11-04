#pragma once

#include <QWidget>
#include "ui_timerendswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TimerEndsWindowClass; };
QT_END_NAMESPACE

class TimerEndsWindow : public QWidget
{
	Q_OBJECT

public:
	TimerEndsWindow(QWidget *parent = nullptr);
	~TimerEndsWindow();

private:
	Ui::TimerEndsWindowClass *ui;

private slots:
	void onCloseWindowPressed();
};

