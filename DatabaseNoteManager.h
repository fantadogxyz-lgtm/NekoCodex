#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QVector>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QDateTime>

#define DATABASE_HOSTNAME   "ExampleDataBase"
#define DATABASE_NAME			"codeka.db"

#define TABLE                   "notes"
#define TABLE_TITLE             "title"
#define TABLE_CONTENT           "content"
#define TABLE_COLOR             "color"
#define TABLE_CREATED_AT        "created_at"
#define TABLE_UPPENDED_AT       "uppended_at"

struct NoteData
{
	int id;
	QString title;
	QString content;
	QDateTime createdAt;
	QDateTime updatedAt;
	QString color;
};

class DatabaseNoteManager
{
	
public:
	DatabaseNoteManager(DatabaseNoteManager& other) = delete;
	void operator=(DatabaseNoteManager other) = delete;

	static DatabaseNoteManager* instance();
	QSqlDatabase& getDB();
	bool initialize();

	int createNote(const QString& title, const QString& content = "", const QString& color = "yellow") const;
	bool updateNote(int id, const QString& title, const QString& content);
	bool updateNoteColor(int id, const QString& color);
	bool deleteNote(int id);
	QVector<NoteData> getAllNotes();
	NoteData getNoteById(int id);
	QString getLastError() const { return m_db.lastError().text(); }

private:
	DatabaseNoteManager() = default;
	~DatabaseNoteManager() = default;

	QString getDataBasePath();
	bool createTable();

	QSqlDatabase m_db;
};
