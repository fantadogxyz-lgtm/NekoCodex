#include "ReminderTimersManager.h"

ReminderTimersManager* ReminderTimersManager::instance() {
    static ReminderTimersManager inst;
    return &inst;
}

bool ReminderTimersManager::initialize() {
    return true;
}

void ReminderTimersManager::startAllReminders()
{
    auto* db = DatabaseReminderManager::instance();

    if (!db->getDB().isOpen()) {
        qDebug() << "[ReminderTimersManager] DB not open, initializing...";
        if (!db->initialize()) {
            qCritical() << "[ReminderTimersManager] Failed to initialize DB!";
            return;
        }
    }

    auto reminders = db->getAllReminders();

    int currentDelayMs = 0;

    for (const auto& r : reminders) {
        m_info[r.id] = { r.id, r.content, r.period };

        startTimerForReminder(r.id, currentDelayMs);

        if (r.period > 0) {
            currentDelayMs += 20000;
        }

        qDebug() << "[ReminderTimersManager] Loaded reminder id =" << r.id
            << "period =" << r.period
            << "content =" << r.content;
    }

    qDebug() << "[ReminderTimersManager] All reminders processed. Total count:" << reminders.size();
}

void ReminderTimersManager::addOrUpdateReminder(int id, const QString& content, int periodMinutes)
{

   
    if (m_info.contains(id)) {
        if (m_info[id].content == content && m_info[id].periodMinutes == periodMinutes && m_timers.contains(id)) {
            qDebug() << "[ReminderTimersManager] Reminder" << id << "not changed. Keeping active timer.";
            return;
        }
    }

    m_info[id] = { id, content, periodMinutes };
    startTimerForReminder(id, 0);
}

void ReminderTimersManager::removeReminder(int id)
{

    if (m_timers.contains(id)) {
        m_timers[id]->stop();
        m_timers[id]->deleteLater();
        m_timers.remove(id);
    }


    m_info.remove(id);
    qDebug() << "[ReminderTimersManager] Reminder id =" << id << "completely removed.";
}

void ReminderTimersManager::startCatFactTimer(int seconds) 
{
    if (!Neko_settings::instance()->isRandomMessagesAllowed()) {
        qDebug() << "[CatFact] WAAAITT!!!! NAN-NA-NA-NA-NA-NA";
        return;
    }
   

    if (!m_catFactTimer) {
        m_catFactTimer = new QTimer(this);
        connect(m_catFactTimer, &QTimer::timeout, this, &ReminderTimersManager::onCatFactTimeout);
    }
    m_catFactTimer->setInterval(seconds * 60000);
    m_catFactTimer->start();

    if (!m_networkManager) {
        m_networkManager = new QNetworkAccessManager(this);
    }
    qDebug() << "[CatFact] timer for " << seconds << " min";
}

void ReminderTimersManager::stopCatFactTimer()
{
    if (m_catFactTimer && m_catFactTimer->isActive()) {
        m_catFactTimer->stop();
        qDebug() << "[CatFact] WAAAITT!!!! NAN-NA-NA-NA-NA-NA.";
    }
}

void ReminderTimersManager::startTimerForReminder(int id, int delayMs)
{
    if (!m_info.contains(id)) return;

    auto info = m_info[id];


    if (m_timers.contains(id)) {
        m_timers[id]->stop();
        m_timers[id]->deleteLater();
        m_timers.remove(id);
    }

    if (info.periodMinutes == 0) {
        qDebug() << "[ReminderTimersManager] Reminder id =" << id << "has period 0 (AFK). Timer disabled.";
        return;
    }

    auto* timer = new QTimer(this);
    timer->setSingleShot(true);

    int periodMs = info.periodMinutes * 60 * 1000;

    timer->setInterval(periodMs + delayMs);

    connect(timer, &QTimer::timeout, this, &ReminderTimersManager::onTimerTimeout);

    m_timers[id] = timer;
    timer->start();

    qDebug() << "Timer started for reminder" << id
        << "with delay" << delayMs / 1000 << "sec";
}

void ReminderTimersManager::onCatFactTimeout()
{
    qDebug() << "[CatFact] zapros za factom";
    QUrl url("https://catfact.ninja/fact");
    QNetworkRequest request(url);

    QNetworkReply* reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        this->onFactFetched(reply);
        });
}

void ReminderTimersManager::onFactFetched(QNetworkReply* reply)
{
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "[CatFact] error: " << reply->errorString();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        QString engFact = jsonObj["fact"].toString();
        qDebug() << "[CatFact] fact (ENG):" << engFact;

      
        QUrl url("https://api.mymemory.translated.net/get");
        QUrlQuery query;
        query.addQueryItem("q", engFact);
        query.addQueryItem("langpair", "en|ru"); // С английского на русский
        url.setQuery(query);

        QNetworkRequest transRequest(url);
        QNetworkReply* transReply = m_networkManager->get(transRequest);

        connect(transReply, &QNetworkReply::finished, this, [this, transReply]() {
            this->onTranslationFetched(transReply);
            });
    }
}

void ReminderTimersManager::onTranslationFetched(QNetworkReply* reply)
{
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "[CatFact] error: " << reply->errorString();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        QJsonObject responseDataObj = jsonObj["responseData"].toObject();

        QString rusFact = responseDataObj["translatedText"].toString();
        m_lastFact = rusFact;
        qDebug() << "[CatFact] fact (RUS):" << m_lastFact;

        auto riminderWindowEmit = new ReminderWindowEmit();
        riminderWindowEmit->setWindowFlags(riminderWindowEmit->windowFlags() | Qt::WindowStaysOnTopHint);
        riminderWindowEmit->setAttribute(Qt::WA_DeleteOnClose);
        riminderWindowEmit->activateWindow();
        riminderWindowEmit->setData(m_lastFact);
        riminderWindowEmit->show();
    }
}

void ReminderTimersManager::onTimerTimeout()
{
    QTimer* timer = qobject_cast<QTimer*>(sender());
    if (!timer) return;

    int id = -1;
    for (auto it = m_timers.begin(); it != m_timers.end(); ++it) {
        if (it.value() == timer) {
            id = it.key();
            break;
        }
    }

    if (id == -1) return;

    auto info = m_info[id];

    qDebug() << "Reminder triggered:" << info.content;

    emit reminderTriggered(id, info.content);

    auto riminderWindowEmit = new ReminderWindowEmit();
    riminderWindowEmit->setAttribute(Qt::WA_DeleteOnClose);
    riminderWindowEmit->show();
    riminderWindowEmit->activateWindow();

    QString text = DatabaseReminderManager::instance()->getReminderTextById(id);

    riminderWindowEmit->setData(text);


    startTimerForReminder(id, 0);
}