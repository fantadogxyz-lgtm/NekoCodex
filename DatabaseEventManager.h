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

#define TABLE                   "events"
#define TABLE_CONTENT           "content"
#define TABLE_CREATED_AT        "created_at"
#define TABLE_REMIND_AT         "remind_at"
#define TABLE_IS_PASSED         "is_passed"

struct EventData
{
	int id;
	QString content;
	QDateTime createdAt;
	QDateTime remindAt;
	bool isPassed = false;
};



class DatabaseEventManager : public QObject
{
	Q_OBJECT

public:
	DatabaseEventManager(DatabaseEventManager& other) = delete;
	void operator=(DatabaseEventManager other) = delete;

	static  DatabaseEventManager* instance();
	QSqlDatabase& getDB();
	bool initialize();



	int createEvent(const QString& content, const QDateTime& remindAt) const;
	bool updateEvent(int id, const QString& content, const QDateTime& remindAt);
	bool deleteEvent(int id);
	QVector<EventData> getAllEvents();
	QVector<EventData> getUpcomingEvents();
	QVector<EventData> getPassedEvents();
	EventData getEventById(int id);
	QString getLastError() const { return m_db.lastError().text(); }
	bool markEventAsPassed(int id);

private:
	DatabaseEventManager() = default;
	~DatabaseEventManager() = default;

	QString getDataBasePath();

	bool createTable();
	QSqlDatabase m_db;

	

};
