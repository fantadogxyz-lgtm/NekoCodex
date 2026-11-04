#pragma once

#include <QObject>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>



#define DATABASE_HOSTNAME   "ExampleDataBase"
#define DATABASE_NAME			"codeka.db"

#define TABLE                   "reminders"
#define TABLE_CONTENT           "content"
#define TABLE_PERIOD             "period"


struct ReminderData
{
	int id;
	QString content;
	int period;
};

class DatabaseReminderManager 
{

public:
	DatabaseReminderManager(DatabaseReminderManager& other) = delete;
	void operator=(DatabaseReminderManager other) = delete;

	static DatabaseReminderManager* instance();
	QSqlDatabase& getDB();
	bool initialize();

	int createReminder(const QString& content, int period);
	bool updateReminderContent(int id, const QString& content);
	bool updateReminderPeriod(int id, int period);
	bool deleteReminder(int id);

	QVector<ReminderData> getAllReminders();

	ReminderData getReminderById(int id);
	QString getReminderTextById(int id);


	
private:
	DatabaseReminderManager() = default;
	~DatabaseReminderManager() = default;

	QString getDataBasePath();
	bool createTable();

	bool insertInitialTestData();

	QSqlDatabase m_db;
};

