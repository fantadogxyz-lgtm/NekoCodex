#include "oneevent.h"

oneevent::oneevent(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::oneevent)
	, m_id(-1)
{
	ui->setupUi(this);

	connect(ui->deleteButton, &QPushButton::clicked,
		this, &oneevent::onDeleteClicked);

	connect(ui->updateButton, &QPushButton::clicked,
		this, &oneevent::onUpdateClicked);
}

oneevent::~oneevent()
{
	delete ui;
}

void oneevent::setData(int id,
	const QString& content,
	const QDateTime& createdAt,
	const QDateTime& remindAt,
	const bool isPassed) {

	m_id = id;
	m_content = content;
	m_createdAt = createdAt;
	m_remindAt = remindAt;
	m_isPassed = isPassed;

	ui->eventContentLabel->setText(m_content);
	ui->labelDate->setText(m_remindAt.date().toString("dd.MM.yyyy"));
	ui->labelTime->setText(m_remindAt.time().toString("hh:mm"));

	
	if (m_isPassed == true) {
		ui->updateButton->setDisabled(true);
		ui->frame->setStyleSheet("background-color: #9e9ca1;"); 
		ui->updateButton->setDisabled(true);
		ui->updateButton->setStyleSheet("background-color: #483f54;");
	}

}

void oneevent::onDeleteClicked() {
	emit eventDeleted(m_id);

}

void oneevent::onUpdateClicked() {
	emit updateRequested(m_id, m_content, m_createdAt, m_remindAt, m_isPassed);
}

int oneevent::getId() const {
	return m_id;
}
QString oneevent::getContent() const {
	return m_content;
}
QDateTime oneevent::getCreatedAt() const {
	return m_createdAt;
}
QDateTime oneevent::getRemindAt() const {
	return m_remindAt;
}
bool oneevent::getIsPassed() const {
	return m_isPassed;
}
