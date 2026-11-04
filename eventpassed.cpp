#include "eventpassed.h"

EventPassed::EventPassed(QWidget *parent)
	: QWidget(parent), ui(new Ui::EventPassed)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_QuitOnClose, false);

	connect(ui->pushOkButton, &QPushButton::clicked,
		this, &EventPassed::onOkButtonClicked);

	connect(ui->pushOkButton_2, &QPushButton::clicked,
		this, &EventPassed::onDeleteButtonClicked);
}

EventPassed::~EventPassed()
{
	delete ui;
}

void EventPassed::setData(int id, QString content, QDateTime remindAt) {
	m_id = id;
	m_content = content;
	m_remindAt = remindAt;

	ui->labelEventContant->setText(content);
	ui->labelEventDate->setText(remindAt.toString("dd.MM.yyyy hh:mm"));
}

void EventPassed::onOkButtonClicked() {
	this->close();
}

void EventPassed::onDeleteButtonClicked() {
	emit eventToDelete(m_id);
	this->close();
}


