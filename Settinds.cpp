#include "Settinds.h"
#include "ui_Settinds.h"

Settinds::Settinds(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Settinds()),
	m_gridLayout(nullptr), m_firstShow(true)
{
	ui->setupUi(this);

	connect(ui->pushButtonCreateReminder, &QPushButton::clicked,
		this, &Settinds::onCreateReminderClicked);


	qDebug() << "=== created SETTINGS:3 ===";


	fillIntervalsComboBox();
	setSettings();

	connect(ui->checkBoxIsPositionFixed, &QCheckBox::clicked,
		this, &Settinds::onCheckBoxIsPositionFixedClicked);
	connect(ui->checkBoxIsActivityLow, &QCheckBox::clicked,
		this, &Settinds::onCheckBoxIsActivityLow);
	connect(ui->checkBoxIsRandomMessagesAllowed, &QCheckBox::clicked,
		this, &Settinds::onCheckBoxIsRandomMessagesAllowed);

	connect(ui->comboBoxForRandomMessages, &QComboBox::currentTextChanged,
		this, [this](const QString& text) {
			bool ok;
			int interval = text.toInt(&ok);
			if (ok) {
				Neko_settings::instance()->setTheIntervalForRandomMessages(interval);
				qDebug() << "Saved random interval:" << interval;
				Neko_settings::instance()->setTheIntervalForRandomMessages(interval);
				Neko_settings::instance()->isRandomMessagesAllowed() ? makeSettingsRandomMessagesDisable(false) : makeSettingsRandomMessagesDisable(true);
			}
		});

	QPixmap fish(":/res/img/settings/codeka-settings.png");
	ui->labelForCodekaSettings->setPixmap(fish);
	ui->labelForCodekaSettings->setScaledContents(true);



}

Settinds::~Settinds()
{
	delete ui;
}

void Settinds::loadReminders()
{
	cleanReminders();
	auto* dbManager = DatabaseReminderManager::instance();
	if (!dbManager->getDB().isOpen()) {
		qDebug() << "loadReminders(): Database not open, initializing...";
		if (!dbManager->initialize()) {
			qCritical() << "loadReminders(): Failed to initialize database!";
			return;
		}
	}

	auto reminders = DatabaseReminderManager::instance()->getAllReminders();

	for (const auto& reminderData : reminders) {
		auto* remind = new Reminder(nullptr);
		qDebug() << "[loadReminders] created Reminder" << remind
			<< "for id =" << reminderData.id
			<< "content =" << reminderData.content;
		remind->setData(reminderData.id, reminderData.content, listOfIntervalsForComboBox2, reminderData.period);
		remind->setFixedSize(REMINDER_WIDTH, REMINDER_HEIGHT);
		connect(remind, &Reminder::contentChangedReminder,
			this, &Settinds::onReminderContentChanged);
	
		connect(remind, &Reminder::periodChangedReminder,
			this, &Settinds::onReminderPeriodChanged);
	
		connect(remind, &Reminder::deleteReminder,
			this, &Settinds::onReminderDeleted);

		ReminderTimersManager::instance()->addOrUpdateReminder(
			reminderData.id,
			reminderData.content,
			reminderData.period
		);

		m_reminders.prepend(remind);
	}

	arrangeReminders();

	qDebug() << "Loaded reminders:" << reminders.size();
}

void Settinds::setSettings()
{
	Neko_settings::instance()->isCodekaFixedOnScreen()   ? ui->checkBoxIsPositionFixed->setChecked(true) : ui->checkBoxIsPositionFixed->setChecked(false);
	Neko_settings::instance()->isCodekaInSlowMode()      ? ui->checkBoxIsActivityLow->setChecked(true)   : ui->checkBoxIsActivityLow->setChecked(false);
	Neko_settings::instance()->isRandomMessagesAllowed() ? makeSettingsRandomMessagesDisable(false)       : makeSettingsRandomMessagesDisable(true);


	
	int savedInterval = Neko_settings::instance()->whatIsTheIntervalForRandomMessages();
	int index = ui->comboBoxForRandomMessages->findText(QString::number(savedInterval));

	if (index != -1) {
		ui->comboBoxForRandomMessages->setCurrentIndex(index);
	}
	else {
		ui->comboBoxForRandomMessages->setCurrentIndex(0);
	}
}

void Settinds::makeSettingsRandomMessagesDisable(bool makeDisable)
{
	if (!makeDisable) {
		ui->checkBoxIsRandomMessagesAllowed->setChecked(true);
		ui->comboBoxForRandomMessages->setEnabled(true);
		ui->comboBoxForRandomMessages->setStyleSheet("background-color: rgb(249, 246, 255); color: rgb(113, 113, 170); ");
		ui->labelForRandomMessages->setStyleSheet("background-color: rgba(255, 255, 255, 0); color: rgb(97, 97, 144); border: none; ");
		Neko_settings::instance()->setRandomMessagesAllowed(true);
		int time = Neko_settings::instance()->whatIsTheIntervalForRandomMessages();
		ReminderTimersManager::instance()->startCatFactTimer(time);
	}
	else {
		ui->checkBoxIsRandomMessagesAllowed->setChecked(false);
		ui->comboBoxForRandomMessages->setEnabled(false);
		ui->comboBoxForRandomMessages->setStyleSheet("background-color: rgb(168, 168, 168); color: rgb(80, 80, 80); ");
		ui->labelForRandomMessages->setStyleSheet("background-color: rgb(168, 168, 168, 0); color: rgb(168, 168, 168);border: none; ");
		Neko_settings::instance()->setRandomMessagesAllowed(false);
		ReminderTimersManager::instance()->stopCatFactTimer();
	}
}

void Settinds::fillIntervalsComboBox()
{
	ui->comboBoxForRandomMessages->clear();
	ui->comboBoxForRandomMessages->addItems(listOfIntervalsForComboBox);
}

void Settinds::arrangeReminders()
{
	if (!m_gridLayout) {
		qDebug() << "[arrangeReminders] m_gridLayout is null";
		return;
	}

	qDebug() << "[arrangeReminders] reminders count =" << m_reminders.size();

	while (auto* item = m_gridLayout->takeAt(0)) {
		delete item;
	}

	for (int i = 0; i < m_reminders.size(); ++i) {
		int row = i / COLUMNS;
		int col = i % COLUMNS;

		qDebug() << "[arrangeReminders] add reminder" << m_reminders[i]
			<< "at" << row << col;

		m_gridLayout->addWidget(m_reminders[i], row, col, Qt::AlignCenter);
	}

	ui->scrollArea->widget()->update();
}

void Settinds::cleanReminders()
{
	for (auto* reminder : m_reminders) {
		reminder->deleteLater();
	}
	m_reminders.clear();
}

void Settinds::setupGrid() {
	QWidget* container = ui->scrollArea->widget();



	if (container->layout()) {
		delete container->layout();
	}

	m_gridLayout = new QGridLayout(container);  

	int scrollWidth = ui->scrollArea->width();
	int totalWidth = scrollWidth - 20;
	int spacing = (totalWidth - (COLUMNS * REMINDER_WIDTH)) / (COLUMNS + 1);

	m_gridLayout->setHorizontalSpacing(spacing);
	m_gridLayout->setVerticalSpacing(20);
	m_gridLayout->setContentsMargins(spacing, 20, spacing, 20);

	for (int i = 0; i < COLUMNS; ++i) {
		m_gridLayout->setColumnStretch(i, 1);
	}

}

void Settinds::onCreateReminderClicked()
{
	int newId = DatabaseReminderManager::instance()->createReminder("(=^*-*^=) ", 0);

	if (newId > 0) {
		auto reminderData = DatabaseReminderManager::instance()->getReminderById(newId);

		auto* reminder = new Reminder(ui->scrollArea->widget());
		reminder->setData(reminderData.id, reminderData.content, listOfIntervalsForComboBox2, reminderData.period);
		reminder->setFixedSize(REMINDER_WIDTH, REMINDER_HEIGHT);

		connect(reminder, &Reminder::contentChangedReminder,
			this, &Settinds::onReminderContentChanged);

		connect(reminder, &Reminder::periodChangedReminder,
			this, &Settinds::onReminderPeriodChanged);

		connect(reminder, &Reminder::deleteReminder,
			this, &Settinds::onReminderDeleted);

		m_reminders.append(reminder);
		arrangeReminders();

		ReminderTimersManager::instance()->addOrUpdateReminder(
			reminderData.id,
			reminderData.content,
			reminderData.period
		);

		QTimer::singleShot(100, this, [this, reminder]() {
			ui->scrollArea->ensureWidgetVisible(reminder);
			qDebug() << "Scrolling to new reminder";
			});
	}
}



void Settinds::onCheckBoxIsPositionFixedClicked()
{	
	ui->checkBoxIsPositionFixed->isChecked() ? Neko_settings::instance()->setCodekaFixedOnScreen(true) : Neko_settings::instance()->setCodekaFixedOnScreen(false);
	ui->checkBoxIsPositionFixed->isChecked() ? emit signalIsPositionFixed(true) : emit signalIsPositionFixed(false);
	
}

void Settinds::onCheckBoxIsActivityLow()
{
	ui->checkBoxIsActivityLow->isChecked() ? Neko_settings::instance()->setCodekaInSlowMode(true) : Neko_settings::instance()->setCodekaInSlowMode(false);
	ui->checkBoxIsActivityLow->isChecked() ? emit signalIsActivityLow(true) : emit signalIsActivityLow(false);
}

void Settinds::onCheckBoxIsRandomMessagesAllowed()
{
	ui->checkBoxIsRandomMessagesAllowed->isChecked() ? Neko_settings::instance()->setRandomMessagesAllowed(true) : Neko_settings::instance()->setRandomMessagesAllowed(false);
	ui->checkBoxIsRandomMessagesAllowed->isChecked() ? makeSettingsRandomMessagesDisable(false) : makeSettingsRandomMessagesDisable(true);
}

void Settinds::onReminderDeleted(int id)
{
	int index = -1;
	for (int i = 0; i < m_reminders.size(); ++i) {
		if (m_reminders[i]->getId() == id) {
			index = i;
			break;
		}
	}

	if (index != -1 && DatabaseReminderManager::instance()->deleteReminder(id)) {
		m_reminders[index]->deleteLater();
		m_reminders.remove(index);
		arrangeReminders();
	}

	ReminderTimersManager::instance()->removeReminder(id);
}


void Settinds::onReminderContentChanged(int id, const QString& content)
{
	if (DatabaseReminderManager::instance()->updateReminderContent(id, content)) {
		
	}
}

void Settinds::onReminderPeriodChanged(int id, int period)
{
	if (DatabaseReminderManager::instance()->updateReminderPeriod(id, period)) {
		QString content;

		for (auto* r : m_reminders) {
			if (r->getId() == id) {
				content = r->getContent(); 
				break;
			}
		}

		ReminderTimersManager::instance()->addOrUpdateReminder(
			id,
			content,
			period
		);
	}
}


void Settinds::showEvent(QShowEvent* event) {
	QWidget::showEvent(event);

	if (m_firstShow) {
		m_firstShow = false;
		setupGrid();
		loadReminders();
		arrangeReminders();
	}
}

