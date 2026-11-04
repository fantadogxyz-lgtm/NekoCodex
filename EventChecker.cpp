#include "EventChecker.h"


EventChecker* EventChecker::instance() {
	static EventChecker m_instanse;
	return &m_instanse;
}

bool EventChecker::initialize() {

    static bool isInitialized = false;
    if (isInitialized) return true;

    qDebug() << "[EventChecker] Immediate first check on startup...";
    checkEvents();

    m_timer = new QTimer(this);
    m_timer->setInterval(60000);

    connect(m_timer, &QTimer::timeout, this, &EventChecker::checkEvents);


    QDateTime now = QDateTime::currentDateTime();
    int msecToNextMinute = 60000 - (now.time().second() * 1000 + now.time().msec());

    qDebug() << "[EventChecker] Synchronization timer armed. Waiting" << msecToNextMinute << "ms until the next clean minute...";

    QTimer* syncTimer = new QTimer(this);
    syncTimer->setSingleShot(true);

    connect(syncTimer, &QTimer::timeout, this, [this, syncTimer]() {
        qDebug() << "[EventChecker] Beautiful time reached! Starting main infinite 60s timer.";

        checkEvents();     
        m_timer->start();   

        syncTimer->deleteLater(); 
        });

    syncTimer->start(msecToNextMinute);

    isInitialized = true;
    return true;
}

void EventChecker::checkEvents() {
    qDebug() << "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ";
    auto* dbManager = DatabaseEventManager::instance();
    if (!dbManager->getDB().isOpen()) {
        qDebug() << "checkEvents(): Database not open, initializing...";
        if (!dbManager->initialize()) {
            qCritical() << "checkEvents(): Failed to initialize database!";
            return;
        }
    }


    QDateTime now = QDateTime::currentDateTime();
    auto uppcomingEvents = dbManager->getUpcomingEvents();

    for (const auto& event : uppcomingEvents) {

        QDateTime remindLocal = event.remindAt.toLocalTime();

        qDebug() << "  ID:" << event.id
            << " | db" << remindLocal.toString("hh:mm:ss")
            << " <= now:" << now.toString("hh:mm:ss");

        if (!event.isPassed && remindLocal <= now) {
            if (!m_notifiedEvents.contains(event.id)) {
                qDebug() << "The event is overdue:" << event.content << " " << event.id;
                dbManager->markEventAsPassed(event.id);
                emit eventPassed(event.id);

                m_notifiedEvents.append(event.id);

                auto eventPassedWidjet = new EventPassed();
                eventPassedWidjet->setAttribute(Qt::WA_DeleteOnClose);


                eventPassedWidjet->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

                eventPassedWidjet->setData(event.id, event.content, event.remindAt);
                eventPassedWidjet->show();
                eventPassedWidjet->raise();
                eventPassedWidjet->activateWindow();

                connect(eventPassedWidjet, &EventPassed::eventToDelete,
                    this, &EventChecker::deleteEvent);
            }
        }
    }
    
    
}

void EventChecker::deleteEvent(int id) {
    auto* dbManager = DatabaseEventManager::instance();

    if (!dbManager->getDB().isOpen()) {
        qDebug() << "EventChecker deleteEvent(): Database not open, initializing...";
        if (!dbManager->initialize()) {
            qCritical() << "EventChecker deleteEvent() : Failed to initialize database!";
            return;
        }
    }

    if (!dbManager->deleteEvent(id)) {
        qCritical() << "EventChecker deleteEvent(): Failed to delete event!";
        return;
    }
    emit eventDeleted(id);
    qDebug() << "EventChecker deleteEvent(): success!";
}

void EventChecker::updateTimer() {
    checkEvents();
}
