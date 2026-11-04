#include "RandomPic.h"
#include "ui_RandomPic.h"

RandomPic::RandomPic(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::RandomPic())
{
	ui->setupUi(this);
	int num = QRandomGenerator::global()->bounded(1, 17); 
	QString path = QString(":/res/img/for_about/randomPic%1.png").arg(num);

	QPixmap pic1(path);
	ui->label->setPixmap(pic1);
	ui->label->setScaledContents(true);
}

RandomPic::~RandomPic()
{
	delete ui;
}
