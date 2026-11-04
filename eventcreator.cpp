#include "eventcreator.h"
#include "ui_eventcreator.h""

EventCreator::EventCreator(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::EventCreator())
{
	ui->setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);

	connect(ui->cancelButton, &QPushButton::clicked,
		this, &EventCreator::onCancelButtonClicked);

	connect(ui->saveButton, &QPushButton::clicked,
		this, &EventCreator::onSaveButtonClicked);
}

void EventCreator::setData(int id,
	const QString& content,
	const QDateTime& createdAt,
	const QDateTime& remindAt,
	const bool isPassed) {

	m_id = id;
	m_content = content;
	m_createdAt = createdAt;
	m_remindAt = remindAt;
	m_isPassed = isPassed;

	if (m_isCreateMode) {
		ui->spinBoxHours->setValue(12);
		ui->spinBoxMinutes->setValue(0);
		ui->calendarWidget->setSelectedDate(QDate::currentDate());
	}
	else {
		ui->spinBoxHours->setValue(m_remindAt.time().hour());
		ui->spinBoxMinutes->setValue(m_remindAt.time().minute());
		ui->eventContent->setText(m_content);
		ui->calendarWidget->setSelectedDate(remindAt.date());
	}
}


EventCreator::~EventCreator()
{
	delete ui;
}

void EventCreator::setCreateMode(bool isCreativeMode) {
	m_isCreateMode = isCreativeMode;

	if (m_isCreateMode) {
		ui->spinBoxHours->setValue(12);
		ui->spinBoxMinutes->setValue(0);
		ui->eventContent->setText("");
		ui->calendarWidget->setSelectedDate(QDate::currentDate().addDays(1));
	}
}

void EventCreator::onCancelButtonClicked() {
	this->close();
}

void EventCreator::onSaveButtonClicked() {
	
	if (ui->eventContent->text().isEmpty()) {
		//предупреждение что не может быть пустым название события пупупум
		QMessageBox msgBox;
		msgBox.setWindowTitle("\u041F\u0440\u0435\u0434\u0443\u043F\u0440\u0435\u0436\u0434\u0435\u043D\u0438\u0435\u0020\u005E\u002A\u002D\u002A\u005E");
		msgBox.setText("\u0422\u0435\u043A\u0441\u0442\u0020\u0441\u043E\u0431\u044B\u0442\u0438\u044F\u0020\u043D\u0435\u0020\u043C\u043E\u0436\u0435\u0442\u0020\u0431\u044B\u0442\u044C\u0020\u043F\u0443\u0441\u0442\u044B\u043C\u0021");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setStyleSheet("QMessageBox { background-color: white; }"
		"QMessageBox QLabel { color: rgb(85, 85, 127); }"
			"QPushButton { "
			"    background-color: rgb(85, 85, 127); " 
			"    color: white; "                        
			"}");

		msgBox.exec();
		return;
	}

	QDate selectedDate = ui->calendarWidget->selectedDate();
	QDate currentDate = QDate::currentDate();
	QTime selectedTime(ui->spinBoxHours->value(), ui->spinBoxMinutes->value());

	if (selectedDate < currentDate ||
		(selectedDate == currentDate && selectedTime <= QTime::currentTime())) {
		//нельзя на прошлое ставить
		QMessageBox msgBox;
		msgBox.setWindowTitle("\u041F\u0440\u0435\u0434\u0443\u043F\u0440\u0435\u0436\u0434\u0435\u043D\u0438\u0435\u0020\u005E\u002A\u002D\u002A\u005E");
		msgBox.setText("\u0414\u0430\u0442\u0430\u0020\u0438\u0020\u0432\u0440\u0435\u043C\u044F\u0020\u043D\u0435\u0020\u043C\u043E\u0436\u0435\u0442\u0020\u0431\u044B\u0442\u044C\u0020\u0432\u0020\u043F\u0440\u043E\u0448\u043B\u043E\u043C\u0021");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setStyleSheet("QMessageBox { background-color: white; }"
			"QMessageBox QLabel { color: rgb(85, 85, 127); }"
			"QPushButton { "
			"    background-color: rgb(85, 85, 127); "
			"    color: white; "
			"}");

		msgBox.exec();
		return;
	}


	if (m_isCreateMode) {

		emit createEvent(ui->eventContent->text(), QDateTime(selectedDate, selectedTime));
		EventChecker::instance()->updateTimer();
		this->close();
	}
	else {
		emit updateEvent(m_id, ui->eventContent->text(), QDateTime(selectedDate, selectedTime));
		EventChecker::instance()->updateTimer();
		this->close();
	}
}



