#include "Reminder.h"

Reminder::Reminder(QWidget *parent)
	: QWidget(parent), m_id(-1), m_period(0), ui(new Ui::Reminder)
{
	ui->setupUi(this);
	m_timer = new QTimer();
	m_timer->setInterval(1000);
	m_timer->setSingleShot(true);



}

Reminder::~Reminder()
{
	delete m_timer;
}

void Reminder::setData(int id, const QString& content, QStringList& listOfIntervalsForComboBox2, int period)
{

	m_id = id; 
	m_content = content;
	m_listOfIntervalsForComboBox = listOfIntervalsForComboBox2;
	m_period = period;
	ui->comboBox->blockSignals(true);

	ui->plainTextEdit->setPlainText(content);
	ui->comboBox->clear();
	ui->comboBox->addItems(m_listOfIntervalsForComboBox);

	int index = ui->comboBox->findText(QString::number(period));
	if (index != -1)
		ui->comboBox->setCurrentIndex(index);


	if (!isConnected) {

		QObject::connect(ui->plainTextEdit, &QPlainTextEdit::textChanged,
			m_timer, [this]() {
				m_timer->start();
			});

		QObject::connect(m_timer, &QTimer::timeout,
			this, &Reminder::onAutoSave);

		QObject::connect(ui->pushButtonDelete, &QPushButton::clicked,
			this, &Reminder::onDeleteClicked);

		QObject::connect(ui->comboBox, &QComboBox::currentIndexChanged,
			this, &Reminder::onPeriodChangedReminder);
		isConnected = true;
	}

	ui->comboBox->blockSignals(false);

	setIsAFK(period == 0);
	
}

int Reminder::getId()
{
	return m_id;
}

QString Reminder::getContent()
{
	return m_content;
}

QStringList Reminder::getListOfIntervalsForComboBox2()
{
	return m_listOfIntervalsForComboBox;
}

int Reminder::getPeriod()
{
	return m_period;
}

void Reminder::setIsAFK(bool isPeriodZero)
{
	if(isPeriodZero) ui->frame->setStyleSheet("background-color: rgb(194, 194, 194);");
	else ui->frame->setStyleSheet("background-color: rgb(215, 217, 255);");
}


void Reminder::onDeleteClicked() {
	emit deleteReminder(m_id);
}
void Reminder::saveChangeColor()	
{
	ui->frame->setStyleSheet("background-color: rgb(138, 207, 142);");
	QTimer::singleShot(1000, this, [this]() {
		setIsAFK(m_period == 0);
	});

}
void Reminder::onAutoSave() {
	m_content = ui->plainTextEdit->toPlainText();
	emit contentChangedReminder(m_id, m_content);
	saveChangeColor();
}


void Reminder::onPeriodChangedReminder()
{
	if (ui->comboBox->currentIndex() == -1) return;
	m_period = ui->comboBox->currentText().toInt();
	setIsAFK(m_period == 0);
	emit periodChangedReminder(m_id, m_period);
	saveChangeColor();
}
