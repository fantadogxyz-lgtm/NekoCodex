#include "AboutProgramInWindow.h"
#include "ui_AboutProgramInWindow.h"

AboutProgramInWindow::AboutProgramInWindow(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::AboutProgramInWindow())
{
	ui->setupUi(this);

	QPixmap pic1(":/res/img/for_about/about_neko_1.jpg");
	ui->labelNeko1->setPixmap(pic1);
	ui->labelNeko1->setScaledContents(true);


	QPixmap pic2(":/res/img/for_about/about_neko_3.png");
	ui->labelNeko2->setPixmap(pic2);
	ui->labelNeko2->setScaledContents(true);

	QPixmap pic3(":/res/img/for_about/about_neko_2.jpg");
	ui->labelNeko3->setPixmap(pic3);
	ui->labelNeko3->setScaledContents(true);
}

AboutProgramInWindow::~AboutProgramInWindow()
{
	delete ui;
}
