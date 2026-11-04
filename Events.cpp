#include "Events.h"
#include "ui_Events.h"

Events::Events(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Events())
{
	ui->setupUi(this);

    setupGrid();

    connect(ui->createEventBut, &QPushButton::clicked,
        this, &Events::onCreateEventClicked);

    connect(ui->showAllButton, &QPushButton::clicked,
        this, &Events::onShowAllClicked);

    connect(ui->showPassedButton, &QPushButton::clicked,
        this, &Events::onShowPassedClicked);

    connect(ui->showUpcomingButton, &QPushButton::clicked,
        this, &Events::onShowUpcomingClicked);

    loadEvents();
}

Events::~Events()
{
	delete ui;
}

void Events::setupGrid() {

    QWidget* container = ui->scrollArea->widget();

    if (container->layout()) {
        delete container->layout();
    }

    m_gridLayout = new QGridLayout(container);

    int scrollWidth = ui->scrollArea->width();
    int totalWidth = scrollWidth - 20; // минус скроллбар

    int spacing = (totalWidth - (COLUMNS * EVENT_WIDTH)) / (COLUMNS + 1);

    m_gridLayout->setHorizontalSpacing(spacing);
    m_gridLayout->setVerticalSpacing(20);
    m_gridLayout->setContentsMargins(spacing, 20, spacing, 20);


    for (int i = 0; i < COLUMNS; ++i) {
        m_gridLayout->setColumnStretch(i, 1);
    }


}


void Events::loadEvents() {
    cleanEvents();

    auto* dbManager = DatabaseEventManager::instance();
    if (!dbManager->getDB().isOpen()) {
        qDebug() << "loadEvents(): Database not open, initializing...";
        if (!dbManager->initialize()) {
            qCritical() << "loadEvents(): Failed to initialize database!";
            return;
        }
    }

    QVector<EventData> allEvents;

    if (showAll) {
        allEvents = DatabaseEventManager::instance()->getAllEvents();
    }
    else if (showPassed) {
        allEvents = DatabaseEventManager::instance()->getPassedEvents();
    }
    else {
        allEvents = DatabaseEventManager::instance()->getUpcomingEvents();
    }
    

    std::sort(allEvents.begin(), allEvents.end(),
        [](const EventData& a, const EventData& b) {
            return a.createdAt > b.createdAt;
        });

    for (const auto& eventData : allEvents) {
        auto* event = new oneevent(this);
        event->setData(eventData.id, eventData.content, eventData.createdAt, eventData.remindAt, eventData.isPassed);

        event->setFixedSize(EVENT_WIDTH, EVENT_HEIGHT);

        connect(event, &oneevent::eventDeleted,
            this, &Events::onEventDeleted);
        connect(event, &oneevent::updateRequested,
            this, &Events::onUpdateEventFromWidget);


        m_allEvents.prepend(event);
    }

    arrangeAllEvents();
}

void Events::cleanEvents() {
    for (auto* event : m_allEvents) {
        event->deleteLater();
    }
    m_allEvents.clear();
}

void Events::arrangeAllEvents(){
    if (!m_gridLayout) {
        return;
    }

    while (auto* item = m_gridLayout->takeAt(0)) {
        m_gridLayout->removeItem(item);

    }

    for (int i = 0; i < m_allEvents.size(); ++i) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;

        m_gridLayout->addWidget(m_allEvents[i], row, col, Qt::AlignCenter);
    }

    if (!m_allEvents.isEmpty()) {
        int lastRow = (m_allEvents.size() - 1) / COLUMNS;
        m_gridLayout->setRowStretch(lastRow + 1, 1);
    }

    ui->scrollArea->widget()->update();
}



void Events::onEventDeleted(int id) {
    int index = -1;
    for (int i = 0; i < m_allEvents.size(); ++i) {
        if (m_allEvents[i]->getId() == id) {
            index = i;
            break;
        }
    }

    if (index != -1 && DatabaseEventManager::instance()->deleteEvent(id)) {
        m_allEvents[index]->deleteLater();
        m_allEvents.remove(index);
        arrangeAllEvents();
    }
    
}

    void Events::onCreateEventClicked() {
        qDebug() << "onCreateEventClicked()";

        if (eventCreator) {
            qDebug() << "EventCreator already exists, bringing to front";
            eventCreator->activateWindow();
            eventCreator->raise();
            return;
        }

        eventCreator = new EventCreator();
        eventCreator->setAttribute(Qt::WA_DeleteOnClose);
        eventCreator->setWindowModality(Qt::WindowModal);
        eventCreator->setWindowTitle("-_-");

        connect(eventCreator, &EventCreator::createEvent,
            this, &Events::onCreateEventSignal);
        connect(eventCreator, &EventCreator::updateEvent,
            this, &Events::onUpdateEventSignal);
        eventCreator->setFixedSize(eventCreator->size());

        connect(eventCreator, &QObject::destroyed, this, [this]() {
            qDebug() << "EventCreator destroyed, nulling pointer";
            eventCreator = nullptr;
            });

        eventCreator->setCreateMode(true);
        eventCreator->show();
        eventCreator->activateWindow();
  
    }

void Events::onCreateEventSignal(const QString& content, const QDateTime& remindAt) {

    int newId = DatabaseEventManager::instance()->createEvent(content, remindAt);

    if (newId > 0) {
        auto eventData = DatabaseEventManager::instance()->getEventById(newId);
        auto* event = new oneevent();
        event->setData(eventData.id, eventData.content,
            eventData.createdAt, eventData.remindAt, eventData.isPassed);
        event->setFixedSize(EVENT_WIDTH, EVENT_HEIGHT);

        connect(event, &oneevent::eventDeleted,
            this, &Events::onEventDeleted);

        connect(event, &oneevent::updateRequested, 
            this, &Events::onUpdateEventFromWidget);

        loadEvents();
       // m_allEvents.append(event);
       // arrangeAllEvents();

        

    }

}

void Events::onUpdateEventFromWidget(int id, const QString& content,
    const QDateTime& createdAt,
    const QDateTime& remindAt,
    bool isPassed) {

    qDebug() << "onUpdateEventFromWidget called for id:" << id;

    if (eventCreator) {
        eventCreator->close();

    }


    eventCreator = new EventCreator();
    eventCreator->setAttribute(Qt::WA_DeleteOnClose);
    eventCreator->setWindowModality(Qt::WindowModal);
    eventCreator->setWindowTitle("0-0");
    eventCreator->setFixedSize(eventCreator->size());

    connect(eventCreator, &EventCreator::createEvent,
        this, &Events::onCreateEventSignal);
    connect(eventCreator, &EventCreator::updateEvent,
        this, &Events::onUpdateEventSignal);


    connect(eventCreator, &QObject::destroyed, this, [this]() {
        qDebug() << "EventCreator destroyed, nulling pointer";
        eventCreator = nullptr;
        });

    eventCreator->setCreateMode(false);
    eventCreator->setData(id, content, createdAt, remindAt, isPassed);
    eventCreator->show();
    eventCreator->activateWindow();

}

void Events::onUpdateEventSignal(int id, const QString& content, const QDateTime& remindAt) {
    if (DatabaseEventManager::instance()->updateEvent(id, content, remindAt)) {
        auto eventData = DatabaseEventManager::instance()->getEventById(id);

        for (int i = 0; i < m_allEvents.size(); ++i) {
            if (m_allEvents[i]->getId() == id) {
                m_gridLayout->removeWidget(m_allEvents[i]);
                m_allEvents[i]->deleteLater();
                m_allEvents.remove(i);
                break;
            }
        }

        auto* event = new oneevent();
        event->setData(eventData.id, eventData.content,
            eventData.createdAt, eventData.remindAt, eventData.isPassed);

        event->setFixedSize(EVENT_WIDTH, EVENT_HEIGHT);

        connect(event, &oneevent::eventDeleted,
            this, &Events::onEventDeleted);
        connect(event, &oneevent::updateRequested, 
            this, &Events::onUpdateEventFromWidget);

        m_allEvents.append(event);
        arrangeAllEvents();

    }
}

void Events::eventPassedSignalFromMenu(int id) {

    auto* dbManager = DatabaseEventManager::instance();
    if (!dbManager->getDB().isOpen()) {
        qDebug() << "loadEvents(): Database not open, initializing...";
        if (!dbManager->initialize()) {
            qCritical() << "loadEvents(): Failed to initialize database!";
            return;
        }
    }

    auto event = dbManager->getEventById(id);
    event.isPassed = false;
    dbManager->markEventAsPassed(id);
    loadEvents();
}

void Events::onShowAllClicked() {
    qDebug() << "onShowAllClicked()";

    ui->showAllButton->setStyleSheet("background-color: rgb(193, 191, 213); color: rgb(255, 255, 255); ");
    ui->showPassedButton->setStyleSheet("background-color: rgb(157, 154, 179); color: rgb(255, 255, 255); ");
    ui->showUpcomingButton->setStyleSheet("background-color: rgb(157, 154, 179); color: rgb(255, 255, 255); ");

    showAll = true;
    showPassed = false;
    showComing = false;
    loadEvents();
}
void Events::onShowUpcomingClicked() {
    qDebug() << "onShowUpcomingClicked()";

    ui->showAllButton->setStyleSheet("background-color: rgb(157, 154, 179); color: rgb(255, 255, 255); ");
    ui->showPassedButton->setStyleSheet("background-color: rgb(157, 154, 179); color: rgb(255, 255, 255); ");
    ui->showUpcomingButton->setStyleSheet("background-color: rgb(193, 191, 213); color: rgb(255, 255, 255); ");

    showAll = false;
    showPassed = false;
    showComing = true;
    loadEvents();
}
void Events::onShowPassedClicked() {
    qDebug() << "onShowPassedClicked()";

    ui->showAllButton->setStyleSheet("background-color: rgb(157, 154, 179); color: rgb(255, 255, 255); ");
    ui->showPassedButton->setStyleSheet("background-color:rgb(193, 191, 213); color: rgb(255, 255, 255); ");
    ui->showUpcomingButton->setStyleSheet("background-color: rgb(157, 154, 179); color: rgb(255, 255, 255); ");

    showAll = false;
    showPassed = true;
    showComing = false;
    loadEvents();
}

void Events::eventDeletedSignalFromMenu(int id) {
    loadEvents();
}

