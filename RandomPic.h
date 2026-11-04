#pragma once

#include <QWidget>
#include <QRandomGenerator>
QT_BEGIN_NAMESPACE
namespace Ui { class RandomPic; };
QT_END_NAMESPACE

class RandomPic : public QWidget
{
	Q_OBJECT

public:
	RandomPic(QWidget *parent = nullptr);
	~RandomPic();

private:
	Ui::RandomPic *ui;
};
