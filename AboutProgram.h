#pragma once

#include <QWidget>
#include "AboutProgramInWindow.h"
#include "RandomPic.h"
#include <QScreen>

QT_BEGIN_NAMESPACE
namespace Ui { class AboutProgram; };
QT_END_NAMESPACE

class AboutProgram : public QWidget
{
	Q_OBJECT

public:
	AboutProgram(QWidget *parent = nullptr);
	~AboutProgram();

private:
	Ui::AboutProgram *ui;
	AboutProgramInWindow* aboutProgramInWindow = nullptr;
};
