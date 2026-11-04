#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#include "DatabaseReminderManager.h"

DatabaseReminderManager* DatabaseReminderManager::instance()
{
	static DatabaseReminderManager m_instance;

	return &m_instance;
}

QSqlDatabase& DatabaseReminderManager::getDB()
{
	return m_db;
}

bool DatabaseReminderManager::initialize()
{
	QString dbPath = getDataBasePath();

	if (m_db.isOpen()) {
		QString connName = m_db.connectionName();
		m_db.close();
		m_db = QSqlDatabase();
		QSqlDatabase::removeDatabase(connName);
	}

	m_db = QSqlDatabase::addDatabase("QSQLITE", "RemindersDB");
	m_db.setDatabaseName(dbPath);

	if (!m_db.open()) {
		return false;
	}

	QSqlQuery query(m_db);
	query.exec("PRAGMA encoding = 'UTF-8'");
	qDebug() << "  :" << dbPath;



	if (!createTable()) {
		return false;
	}
	return true;

}
	

int DatabaseReminderManager::createReminder(const QString& content, int period)
{
	if (!m_db.isOpen()) {
		qCritical() << "createReminder() notes - database is not open";
		return false;
	}

	QSqlQuery query(m_db);
	query.prepare("INSERT INTO " TABLE " (" TABLE_CONTENT
		", " TABLE_PERIOD
		") VALUES(:content, :period)");
	query.bindValue(":content", content);
	query.bindValue(":period", period);

	if (!query.exec()) {
		qDebug() << "createNote() notes - was falled";
		qCritical() << query.lastError().text();
		return -1;
	}
	return query.lastInsertId().toInt();


}


bool DatabaseReminderManager::updateReminderContent(int id, const QString& content)
{
	if (!m_db.isOpen()) {
		qCritical() << "updateReminderContent() - database is not open";
		return false;
	}

	QSqlQuery query(m_db);
	query.prepare("UPDATE " TABLE " SET " 
		TABLE_CONTENT " = :content "
		"WHERE id = :id");

	query.bindValue(":content", content);
	query.bindValue(":id", id);

	if (!query.exec()) {
		QString error = QString("updateReminderContent() failed for ID %1: %2")
			.arg(id)
			.arg(query.lastError().text());
		qCritical() << error;
		return false;
	}
	qDebug() << "updateReminderContent() notes - success";
	return true;

}

bool DatabaseReminderManager::updateReminderPeriod(int id, int period)
{
	if (!m_db.isOpen()) {
		qCritical() << "updateReminderPeriod() - database is not open";
		return false;
	}

	QSqlQuery query(m_db);
	query.prepare("UPDATE " TABLE " SET " 
		TABLE_PERIOD " = :period "
		"WHERE id = :id");

	query.bindValue(":period", period);
	query.bindValue(":id", id);

	if (!query.exec()) {
		QString error = QString("updateReminderPeriod() failed for ID %1: %2")
			.arg(id)
			.arg(query.lastError().text());
		qCritical() << error;
		return false;
	}
	qDebug() << "updateReminderPeriod() notes - success";
	return true;

}


bool DatabaseReminderManager::deleteReminder(int id)
{
	if (!m_db.isOpen()) {
		qCritical() << "deleteReminder() notes - database is not open";
		return false;
	}

	QSqlQuery query(m_db);
	query.prepare("DELETE FROM " TABLE " WHERE id = :id");
	query.bindValue(":id", id);

	if (!query.exec()) {
		QString error = QString("deleteReminder(ID: %1) failed: %2")
			.arg(id)
			.arg(query.lastError().text());
		qCritical() << error;
		return false;
	}
	qDebug() << "deleteReminder(): Reminder" << id << "deleted successfully";
	return true;

}

QVector<ReminderData> DatabaseReminderManager::getAllReminders()
{
	QVector<ReminderData> remindersVec;

	if (!m_db.isOpen()) {
		qCritical() << "getAllNotes() - db is not open";
		return remindersVec;
	}

	QSqlQuery query(m_db);

	QString selectQuery = "SELECT id, "
		TABLE_CONTENT ", "
		TABLE_PERIOD 
		" FROM " TABLE;

	if (!query.exec(selectQuery)) {
		qCritical() << "getAllReminders() failed:" << query.lastError().text();
		return remindersVec;
	}

	while (query.next()) {
		ReminderData reminder;
		reminder.id = query.value("id").toInt();
		reminder.content = query.value(TABLE_CONTENT).toString();
		reminder.period = query.value(TABLE_PERIOD).toInt();
		remindersVec.append(reminder);
	}
	qDebug() << "getAllReminders()  - success";

	return remindersVec;

}

ReminderData DatabaseReminderManager::getReminderById(int id)
{
	ReminderData reminder;
	reminder.id = -1;

	if (!m_db.isOpen()) {
		qCritical() << "getReminderById() - db is not open";
		return reminder;
	}

	QSqlQuery query(m_db);
	query.prepare("SELECT id, "
		TABLE_CONTENT ", "
		TABLE_PERIOD 
		" FROM " TABLE
		" WHERE id = :id");
	query.bindValue(":id", id);

	if (!query.exec()) {
		qCritical() << "getReminderById(" << id << ") failed:"
			<< query.lastError().text();
		return reminder;
	}


	if (query.next()) {
		reminder.id = query.value("id").toInt();
		reminder.content = query.value(TABLE_CONTENT).toString();
		reminder.period = query.value(TABLE_PERIOD).toInt();
		qDebug() << "getReminderById(): Found reminder" << id;
	}
	else {
		qDebug() << "getReminderById(): Reminder " << id << "not found";
	}

	return reminder;

}

QString DatabaseReminderManager::getReminderTextById(int id)
{
	if (!m_db.isOpen()) {
		qCritical() << "getReminderById() - db is not open";
		return "";
	}

	QSqlQuery query(m_db);
	query.prepare("SELECT "
		TABLE_CONTENT 
		" FROM " TABLE
		" WHERE id = :id");
	query.bindValue(":id", id);

	if (!query.exec()) {
		qCritical() << "getReminderById(" << id << ") failed:"
			<< query.lastError().text();
		return "";
	}
	if (query.next()) {
		return query.value(0).toString();
	}

	qWarning() << "getReminderById(" << id << ") - record not found";
	return "";

}

QString DatabaseReminderManager::getDataBasePath()
{
	QString dbPath = QCoreApplication::applicationDirPath();

	QDir dir(dbPath);
	if (!dir.exists()) {
		dir.mkpath(".");
	}

	QString dbFile = dbPath + "/reminders.db";

	qDebug().noquote() << QString("   : %1").arg(dbFile);

	return dbFile;

}

bool DatabaseReminderManager::createTable()
{
	if (!m_db.isOpen()) {
		qCritical() << "createTable() notes - db is not open";
		return false;
	}

	QSqlQuery query(m_db);

	QString createQuery = "CREATE TABLE IF NOT EXISTS " TABLE " ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		TABLE_CONTENT				" TEXT, "
		TABLE_PERIOD				" INTEGER"
		");";

	qDebug() << "Executing query:" << createQuery;
	if (!query.exec(createQuery)) {
		qDebug() << "createTable() notes was falled";
		qDebug() << query.lastError().text();
		return false;
	}
	else {
		qDebug() << "created Table reminders  -  success!";
		//return true;
	}


		query.exec("SELECT COUNT(*) FROM " TABLE);
		if (query.next() && query.value(0).toInt() == 0) {
			return insertInitialTestData();
		}
	
return true;

}

bool DatabaseReminderManager::insertInitialTestData()
{
	
	QSqlQuery query(m_db);

	qDebug() << "Inserting initial test data...";
	query.prepare("INSERT INTO " TABLE " (" TABLE_CONTENT ", " TABLE_PERIOD ") "
		"VALUES(:content, :period)");

	
	
	// Вместо QString c = "...";
	QString s = QString::fromUtf8("\xD0\x9C\xD1\x8F\xD1\x83-\xD0\xBC\xD1\x8F\xD1\x83! \xD0\x92\xD1\x80\xD0\xB5\xD0\xBC\xD1\x8F \xD0\xBF\xD0\xBE\xD0\xB4\xD0\xB2\xD0\xB8\xD0\xB3\xD0\xB0\xD1\x82\xD1\x8C\xD1\x81\xD1\x8F \xD0\xB8 \xD1\x80\xD0\xB0\xD0\xB7\xD0\xBC\xD1\x8F\xD1\x82\xD1\x8C \xD0\xBB\xD0\xB0\xD0\xBF\xD0\xBA\xD0\xB8 <3");

	query.bindValue(":content", s);
	query.bindValue(":period", 30);

	if (!query.exec()) {
		qCritical() << "Failed to insert test reminder 1:" << query.lastError().text();
		return false;
	}

	query.prepare("INSERT INTO " TABLE " (" TABLE_CONTENT ", " TABLE_PERIOD ") "
		"VALUES(:content, :period)");
	QString c = QString::fromUtf8("Weow! \xD0\x9D\xD0\xB0\xD0\xB4\xD0\xBE \xD0\xB1\xD1\x8B \xD0\xBF\xD0\xBE\xD0\xBF\xD0\xB8\xD1\x82\xD1\x8C \xD0\xB2\xD0\xBE\xD0\xB4\xD0\xB8\xD1\x87\xD0\xBA\xD0\xB8 ^-^");

	query.bindValue(":content", c);
	query.bindValue(":period", 45);

	if (!query.exec()) {
		qCritical() << "Failed to insert test reminder 2:" << query.lastError().text();
		return false;
	}

	return true;
}