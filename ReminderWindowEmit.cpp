#include "ReminderWindowEmit.h"
#include "ui_ReminderWindowEmit.h"

ReminderWindowEmit::ReminderWindowEmit(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ReminderWindowEmit())
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);

	connect(ui->pushButtonClose, &QPushButton::clicked,
		this, &ReminderWindowEmit::onOkButtonClicked);

}

ReminderWindowEmit::~ReminderWindowEmit()
{
	delete ui;
}

void ReminderWindowEmit::setData(QString& text)
{
	reminder_text = text;
	ui->plainTextEdit->setPlainText(text);

}


void ReminderWindowEmit::onOkButtonClicked() {
	this->close();
}