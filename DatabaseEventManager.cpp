#include "DatabaseEventManager.h"




DatabaseEventManager* DatabaseEventManager::instance() {
	static DatabaseEventManager m_instanse;

	return &m_instanse;
}

QSqlDatabase& DatabaseEventManager::getDB() {
	return m_db;
}

bool DatabaseEventManager::initialize() {

	QString dbPath = getDataBasePath();

	if (m_db.isOpen()) {
		QString connName = m_db.connectionName();
		m_db.close();
		m_db = QSqlDatabase();
		QSqlDatabase::removeDatabase(connName);
	}

	m_db = QSqlDatabase::addDatabase("QSQLITE", "EventsDB");
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


QString DatabaseEventManager::getDataBasePath() {
	QString dbPath = QCoreApplication::applicationDirPath();

	QDir dir(dbPath);
	if (!dir.exists()) {
		dir.mkpath(".");
	}

	QString dbFile = dbPath + "/events.db";

	qDebug().noquote() << QString("db will be in: %1").arg(dbFile);

	return dbFile;
}


bool DatabaseEventManager::createTable() {

	if (!m_db.isOpen()) {
		qCritical() << "createTable() events - db is not open";
		return false;
	}

	QSqlQuery query(m_db);


	QString createQuery = "CREATE TABLE IF NOT EXISTS " TABLE " ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		TABLE_CONTENT				" TEXT, "
		TABLE_CREATED_AT			" TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
		TABLE_REMIND_AT				" TIMESTAMP NOT NULL, "
		TABLE_IS_PASSED				" INTEGER DEFAULT 0)";

	qDebug() << "Executing query:" << createQuery;

	if (!query.exec(createQuery)) {
		qDebug() << "createTable() events was falled";
		qDebug() << query.lastError().text();
		return false;
	}
	qDebug() << "created Table notes  -  success!";
	return true;
	
}

int DatabaseEventManager::createEvent(const QString& content, const QDateTime& remindAt) const{
	
	if (!m_db.isOpen()) {
		qCritical() << "createNote() events - database is not open";
		return -1;
	}

	QSqlQuery query(m_db);
	query.prepare("INSERT INTO " TABLE " ("
			TABLE_CONTENT
		", " TABLE_REMIND_AT
		") VALUES(:content, :remind_at)");
	query.bindValue(":content", content);
	query.bindValue(":remind_at",remindAt );


	if (!query.exec()) {
		qDebug() << "createEvent() notes - was falled";
		qCritical() << query.lastError().text();
		return -1;
	}
	return query.lastInsertId().toInt();
}

bool  DatabaseEventManager::updateEvent(int id, const QString& content, const QDateTime& remindAt) {
	
	if (!m_db.isOpen()) {
		qCritical() << "updateNote() events - database is not open";
		return -1;
	}

	QSqlQuery query(m_db);
	query.prepare("UPDATE " TABLE " SET "
		TABLE_CONTENT " = :content, "
		TABLE_REMIND_AT " = :remind_at "
		"WHERE id = :id");

	query.bindValue(":content", content);
	query.bindValue(":remind_at", remindAt);
	query.bindValue(":id", id);

	if (!query.exec()) {
		QString error = QString("updateEvent() failed for ID %1: %2")
			.arg(id)
			.arg(query.lastError().text());
		qCritical() << error;
		return false;
	}
	qDebug() << "updateEvent() events - success";
	return true;

}

bool  DatabaseEventManager::deleteEvent(int id) {

	if (!m_db.isOpen()) {
		qCritical() << "deleteEvent() events - database is not open";
		return false;
	}

	QSqlQuery query(m_db);
	query.prepare("DELETE FROM " TABLE " WHERE id = :id");
	query.bindValue(":id", id);

	if (!query.exec()) {
		QString error = QString("deleteEvent(ID: %1) failed: %2")
			.arg(id)
			.arg(query.lastError().text());
		qCritical() << error;
		return false;
	}
	qDebug() << "deleteEvent(): events" << id << "deleted successfully";
	return true;
}

QVector<EventData>   DatabaseEventManager::getAllEvents() {
	QVector<EventData> eventsVec;

	if (!m_db.isOpen()) {
		qCritical() << "getAllEvets() - db is not open";
		return eventsVec;
	}

	QSqlQuery query(m_db);

	QString selectQuery = "SELECT id, "
		TABLE_CONTENT ", "
		TABLE_CREATED_AT ", "
		TABLE_REMIND_AT ", "
		TABLE_IS_PASSED " FROM " TABLE
		" ORDER BY " TABLE_CREATED_AT " DESC";

	if (!query.exec(selectQuery)) {
		qCritical() << "getAllEvets() failed:" << query.lastError().text();
		return eventsVec;
	}

	while (query.next()) {
		EventData event;
		event.id = query.value("id").toInt();
		event.content = query.value(TABLE_CONTENT).toString();

		QDateTime dbCreated = query.value(TABLE_CREATED_AT).toDateTime();
		event.createdAt = dbCreated;

		QDateTime dbRemindAt = query.value(TABLE_REMIND_AT).toDateTime();
		event.remindAt = dbRemindAt;

		event.isPassed = query.value(TABLE_IS_PASSED).toBool();
		eventsVec.append(event);
	}
	return eventsVec;

}

QVector<EventData> DatabaseEventManager::getUpcomingEvents() {
	QVector<EventData> eventsVec;

	if (!m_db.isOpen()) {
		qCritical() << "getUpcomingEvents() - db is not open";
		return eventsVec;
	}

	QSqlQuery query(m_db);

	QString selectQuery = "SELECT id, "
		TABLE_CONTENT ", "
		TABLE_CREATED_AT ", "
		TABLE_REMIND_AT ", "
		TABLE_IS_PASSED " FROM " TABLE
		" WHERE " TABLE_IS_PASSED " = 0 "
		"ORDER BY " TABLE_CREATED_AT " DESC";

	if (!query.exec(selectQuery)) {
		qCritical() << "getUpcomingEvents() failed:" << query.lastError().text();
		return eventsVec;
	}

	while (query.next()) {
		EventData event;
		event.id = query.value("id").toInt();
		event.content = query.value(TABLE_CONTENT).toString();

		QDateTime dbCreated = query.value(TABLE_CREATED_AT).toDateTime();
		event.createdAt = dbCreated;

		QDateTime dbRemindAt = query.value(TABLE_REMIND_AT).toDateTime();
		event.remindAt = dbRemindAt;

		event.isPassed = query.value(TABLE_IS_PASSED).toBool();
		eventsVec.append(event);
	}

	return eventsVec;
}

QVector<EventData> DatabaseEventManager::getPassedEvents() {
	QVector<EventData> eventsVec;

	if (!m_db.isOpen()) {
		qCritical() << "getUpcomingEvents() - db is not open";
		return eventsVec;
	}

	QSqlQuery query(m_db);

	QString selectQuery = "SELECT id, "
		TABLE_CONTENT ", "
		TABLE_CREATED_AT ", "
		TABLE_REMIND_AT ", "
		TABLE_IS_PASSED " FROM " TABLE
		" WHERE " TABLE_IS_PASSED " = 1 "
		"ORDER BY " TABLE_CREATED_AT " DESC";

	if (!query.exec(selectQuery)) {
		qCritical() << "getPassedEvents() failed:" << query.lastError().text();
		return eventsVec;
	}

	while (query.next()) {
		EventData event;
		event.id = query.value("id").toInt();
		event.content = query.value(TABLE_CONTENT).toString();

		QDateTime dbCreated = query.value(TABLE_CREATED_AT).toDateTime();
		event.createdAt = dbCreated;

		QDateTime dbRemindAt = query.value(TABLE_REMIND_AT).toDateTime();
		event.remindAt = dbRemindAt;

		event.isPassed = query.value(TABLE_IS_PASSED).toBool();
		eventsVec.append(event);
	}

	return eventsVec;
}

EventData DatabaseEventManager::getEventById(int id) {
	EventData event;
	event.id = -1;

	if (!m_db.isOpen()) {
		qCritical() << "getEventById() - db is not open";
		return event;
	}

	QSqlQuery query(m_db);

	query.prepare("SELECT id, "
		TABLE_CONTENT ", "
		TABLE_CREATED_AT ", "
		TABLE_REMIND_AT ", "
		TABLE_IS_PASSED " FROM " TABLE
		" WHERE id = :id "
		"ORDER BY " TABLE_CREATED_AT " DESC");
	query.bindValue(":id", id);

	if (!query.exec()) {
		qCritical() << "getEventById(" << id << ") failed:"
			<< query.lastError().text();
		return event;
	}

	if (query.next()) {
		event.id = query.value("id").toInt();
		event.content = query.value(TABLE_CONTENT).toString();

		QDateTime dbCreated = query.value(TABLE_CREATED_AT).toDateTime();
		event.createdAt = dbCreated;

		QDateTime dbRemindAt = query.value(TABLE_REMIND_AT).toDateTime();
		event.remindAt = dbRemindAt;

		event.isPassed = query.value(TABLE_IS_PASSED).toBool();
		
		qDebug() << "getEventById(): Found event";
	}
	else {
		qDebug() << "getEventById(): not found(((";
	}

	return event;
}

bool DatabaseEventManager::markEventAsPassed(int id)
{
	if (!m_db.isOpen()) {
		qCritical() << "markEventAsPassed() - database is not open";
		return false;
	}

	QSqlQuery query(m_db);
	query.prepare("UPDATE " TABLE " SET "
		TABLE_IS_PASSED " = 1 "
		"WHERE id = :id");
	query.bindValue(":id", id);

	if (!query.exec()) {
		QString error = QString("markEventAsPassed() failed for ID %1: %2")
			.arg(id)
			.arg(query.lastError().text());
		qCritical() << error;
		return false;
	}

	qDebug() << "markEventAsPassed(): event" << id << "marked as passed";
	return true;
}

