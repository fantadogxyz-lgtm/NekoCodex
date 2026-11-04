#include "DatabaseNoteManager.h"

DatabaseNoteManager* DatabaseNoteManager::instance()
{
	static DatabaseNoteManager m_instance;

	return &m_instance;
}

QSqlDatabase& DatabaseNoteManager::getDB()
{
	return m_db;
}

bool DatabaseNoteManager::initialize()
{
	QString dbPath = getDataBasePath();
  
	if (m_db.isOpen()) {
		QString connName = m_db.connectionName();
		m_db.close();
		m_db = QSqlDatabase();
		QSqlDatabase::removeDatabase(connName);
	}

	m_db = QSqlDatabase::addDatabase("QSQLITE", "NotesDB");
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

int DatabaseNoteManager::createNote(const QString& title, const QString& content, const QString& color) const
{
	if (!m_db.isOpen()) {
		qCritical() << "createNote() notes - database is not open";
		return false;
	}

	QSqlQuery query(m_db);
	query.prepare("INSERT INTO " TABLE " (" TABLE_TITLE
		", " TABLE_CONTENT 
		", " TABLE_COLOR 
		") VALUES(:title, :content, :color)");
	query.bindValue(":title", title);
	query.bindValue(":content", content);
	query.bindValue(":color", color);

	if (!query.exec()) {
		qDebug() << "createNote() notes - was falled";
		qCritical() << query.lastError().text();
		return -1;
	}
	return query.lastInsertId().toInt();

	
}

bool DatabaseNoteManager::updateNote(int id, const QString& title, const QString& content)
{
	if (!m_db.isOpen()) {
		qCritical() << "createNote() notes - database is not open";
		return false;
	}

	QSqlQuery query(m_db);
	query.prepare("UPDATE " TABLE " SET "
		TABLE_TITLE " = :title, "
		TABLE_CONTENT " = :content "
		"WHERE id = :id");

	query.bindValue(":title", title);
	query.bindValue(":content", content);
	query.bindValue(":id", id);

	if (!query.exec()) {
		QString error = QString("updateNote() failed for ID %1: %2")
			.arg(id)
			.arg(query.lastError().text());
		qCritical() << error;
		return false;
	}
	qDebug() << "updateNote() notes - success";
	return true;

}

bool DatabaseNoteManager::updateNoteColor(int id, const QString& color)
{
	if (!m_db.isOpen()) {
		qCritical() << "createNote() notes - database is not open";
		return false;
	}

	QSqlQuery query(m_db);

	query.prepare("UPDATE " TABLE " SET "
		TABLE_COLOR " = :color "
		"WHERE id = :id");

	query.bindValue(":color", color.isEmpty() ? "yellow" : color);
	query.bindValue(":id", id);

	if (!query.exec()) {
		QString error = QString("updateNoteColor(ID: %1) failed: %2")
			.arg(id)
			.arg(query.lastError().text());
		qCritical() << error;
		return false;
	}
	qDebug() << "updateNoteColor(): Note" << id << "color updated to" << color;
	return true;

}

bool DatabaseNoteManager::deleteNote(int id)
{
	if (!m_db.isOpen()) {
		qCritical() << "createNote() notes - database is not open";
		return false;
	}

	QSqlQuery query(m_db);
	query.prepare("DELETE FROM " TABLE " WHERE id = :id");
	query.bindValue(":id", id);

	if (!query.exec()) {
		QString error = QString("deleteNote(ID: %1) failed: %2")
			.arg(id)
			.arg(query.lastError().text());
		qCritical() << error;
		return false;
	}
	qDebug() << "deleteNote(): Note" << id << "deleted successfully";
	return true;

}

QVector<NoteData> DatabaseNoteManager::getAllNotes()
{
	QVector<NoteData> notesVec;

	if (!m_db.isOpen()) {
		qCritical() << "getAllNotes() - db is not open";
		return notesVec;
	}

	QSqlQuery query(m_db);

	QString selectQuery = "SELECT id, "
		TABLE_TITLE ", "
		TABLE_CONTENT ", "
		TABLE_CREATED_AT ", "
		TABLE_UPPENDED_AT ", "
		TABLE_COLOR " FROM " TABLE
		" ORDER BY " TABLE_CREATED_AT " DESC";

	if (!query.exec(selectQuery)) {
		qCritical() << "getAllNotes() failed:" << query.lastError().text();
		return notesVec;
	}

	while (query.next()) {
		NoteData note;
		note.id = query.value("id").toInt();
		note.title = query.value(TABLE_TITLE).toString();
		note.content = query.value(TABLE_CONTENT).toString();
		note.createdAt = query.value(TABLE_CREATED_AT).toDateTime();

		QDateTime dbCreated = query.value(TABLE_CREATED_AT).toDateTime();
		note.createdAt = dbCreated;

		QDateTime dbUpated = query.value(TABLE_UPPENDED_AT).toDateTime();
		note.updatedAt = dbUpated;

		note.color = query.value(TABLE_COLOR).toString();
		notesVec.append(note);
	}
	qDebug() << "getAllNotes() notes - success";

	return notesVec;

}

NoteData DatabaseNoteManager::getNoteById(int id)
{
	NoteData note;
	note.id = -1;     

	if (!m_db.isOpen()) {
		qCritical() << "getAllNotes() - db is not open";
		return note;
	}

	QSqlQuery query(m_db);
	query.prepare("SELECT id, "
		TABLE_TITLE ", "
		TABLE_CONTENT ", "
		TABLE_CREATED_AT ", "
		TABLE_UPPENDED_AT ", "
		TABLE_COLOR " FROM " TABLE
		" WHERE id = :id");
	query.bindValue(":id", id);

	if (!query.exec()) {
		qCritical() << "getNoteById(" << id << ") failed:"
			<< query.lastError().text();
		return note;
	}


	if (query.next()) {
		note.id = query.value("id").toInt();
		note.title = query.value(TABLE_TITLE).toString();
		note.content = query.value(TABLE_CONTENT).toString();
		note.createdAt = query.value(TABLE_CREATED_AT).toDateTime();
		note.updatedAt = query.value(TABLE_UPPENDED_AT).toDateTime();
		note.color = query.value(TABLE_COLOR).toString();
		qDebug() << "getNoteById(): Found note" << id;
	}
	else {
		qDebug() << "getNoteById(): Note" << id << "not found";
	}

	return note;

}

QString DatabaseNoteManager::getDataBasePath()
{
	QString dbPath = QCoreApplication::applicationDirPath();

	QDir dir(dbPath);
	if (!dir.exists()) {
		dir.mkpath(".");
	}

	QString dbFile = dbPath + "/notes.db";
  
	qDebug().noquote() << QString("   : %1").arg(dbFile);
  
	return dbFile;

}

bool DatabaseNoteManager::createTable()
{
	if (!m_db.isOpen()) {
		qCritical() << "createTable() notes - db is not open";
		return false;
	}

	QSqlQuery query(m_db);

	QString createQuery = "CREATE TABLE IF NOT EXISTS " TABLE " ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		TABLE_TITLE					" TEXT NOT NULL DEFAULT 'Новая заметка', "
		TABLE_CONTENT				" TEXT, "
		TABLE_COLOR					" VARCHAR(20) NOT NULL DEFAULT 'yellow', "
		TABLE_CREATED_AT			" TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
		TABLE_UPPENDED_AT			" TIMESTAMP DEFAULT CURRENT_TIMESTAMP)";

	qDebug() << "Executing query:" << createQuery;
	if (!query.exec(createQuery)) {
		qDebug() << "createTable() notes was falled";
		qDebug() << query.lastError().text();
		return false;
	}
	else {
		qDebug() << "created Table notes  -  success!";
		//return true;
	}


	QString createTriggerQuery = R"(
        CREATE TRIGGER IF NOT EXISTS uppend_notes_timestamp 
        AFTER UPDATE ON notes
        BEGIN
            UPDATE notes SET uppended_at = CURRENT_TIMESTAMP WHERE id = NEW.id;
        END
    )";

	if (!query.exec(createTriggerQuery)) {
		qDebug() << "create trigger in notes - was falled";
		qDebug() << query.lastError().text();
		return false;
	}

	return true;

}
