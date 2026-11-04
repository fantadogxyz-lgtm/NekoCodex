#pragma once

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

#include "DatabaseReminderManager.h"
#include "ReminderWindowEmit.h"
#include "Neko_settings.h"

struct ReminderTimerInfo {
    int id;
    QString content;
    int periodMinutes;
};

class ReminderTimersManager : public QObject
{
    Q_OBJECT

public:
    ReminderTimersManager(ReminderTimersManager&) = delete;
    void operator=(ReminderTimersManager&) = delete;

    static ReminderTimersManager* instance();
    bool initialize();

    void addOrUpdateReminder(int id, const QString& content, int periodMinutes);
    void startAllReminders();
    void removeReminder(int id);

    void startCatFactTimer(int seconds);
    void stopCatFactTimer();

private:
    ReminderTimersManager() = default;
    ~ReminderTimersManager() = default;

    QMap<int, QTimer*> m_timers;
    QMap<int, ReminderTimerInfo> m_info;

    int nextDelayMs = 0; 

    void startTimerForReminder(int id, int delayMs);

    QTimer* m_catFactTimer = nullptr;
    QNetworkAccessManager* m_networkManager = nullptr;
    QString m_lastFact;

private slots:
    void onTimerTimeout();

    void onCatFactTimeout();
    void onFactFetched(QNetworkReply* reply);
    void onTranslationFetched(QNetworkReply* reply);

signals:
    void reminderTriggered(int id, QString content);
};
