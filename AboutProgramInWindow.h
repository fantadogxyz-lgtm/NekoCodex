#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class AboutProgramInWindow; };
QT_END_NAMESPACE

class AboutProgramInWindow : public QWidget
{
	Q_OBJECT

public:
	AboutProgramInWindow(QWidget *parent = nullptr);
	~AboutProgramInWindow();

private:
	Ui::AboutProgramInWindow *ui;
};
