#include "onenote.h"
#include "ui_onenote.h"

OneNote::OneNote(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::OneNote)
    , m_id(-1)
    , m_autoSavingTimer(new QTimer(this))  
{
    ui->setupUi(this);
    ui->noteTitle->setMaxLength(15);
    ui->noteTitle->setReadOnly(false);
    ui->noteContent->setReadOnly(false);

    m_autoSavingTimer->setSingleShot(true);
    m_autoSavingTimer->setInterval(1000); //это 1 секунда есчто:)

    
    //так заметка: у QPlainTextEdit есть по умолчанию скроллл

    //вот тут будут подключения к сигналам

    //таймер для запуска 
    QObject::connect(ui->noteTitle, &QLineEdit::textChanged,
        m_autoSavingTimer, [this]() {
            m_autoSavingTimer->start();
        }); 

    QObject::connect(ui->noteContent, &QPlainTextEdit::textChanged,
        m_autoSavingTimer, [this]() {
            m_autoSavingTimer->start();
        });

    //смена цветов
    QObject::connect(ui->colorYellow, &QPushButton::clicked,
        this, &OneNote::colorChangeYellow);

    QObject::connect(ui->colorBlue, &QPushButton::clicked,
        this, &OneNote::colorChangeBlue);

    QObject::connect(ui->colorGreen, &QPushButton::clicked,
        this, &OneNote::colorChangeGreen);

    QObject::connect(ui->colorPink, &QPushButton::clicked,
        this, &OneNote::colorChangePink);

    QObject::connect(ui->colorOrange, &QPushButton::clicked,
        this, &OneNote::colorChangeOrange);

    //автосохранение
    QObject::connect(m_autoSavingTimer, &QTimer::timeout,
        this, &OneNote::onAutoSave);

    //удалить
    QObject::connect(ui->deleteButt, &QPushButton::clicked,
        this, &OneNote::onDeleteClicked);

}

OneNote::~OneNote()
{
    delete ui;
}


//background-color: rgb(221, 222, 144);


void OneNote::setData(int id, const QString& title,
    const QString& content,
    const QDateTime& createdAt,
    const QDateTime& updatedAt,
    const QString& color) {

    m_id = id;

    m_createdAt = createdAt.addSecs(3 * 3600);
    m_updatedAt = updatedAt.addSecs(3 * 3600);

    m_color = color;

    ui->noteTitle->blockSignals(true);
    ui->noteContent->blockSignals(true);

    ui->noteTitle->setText(title);
    ui->noteContent->setPlainText(content);

    ui->noteTitle->blockSignals(false);
    ui->noteContent->blockSignals(false);

    ui->noteCreatedAt ->setText(m_createdAt.toString("dd.MM.yyyy hh:mm"));
    ui->noteUpdatedAt->setText(m_updatedAt.toString("dd.MM.yyyy hh:mm"));

    setColor(color);
  
}

void OneNote::onAutoSave() {
    emit noteChanged(m_id, getTitle(), getContent());
    
    ui->frameBgColor->setStyleSheet("QFrame{background-color: #bcd4bc; border-radius: 10px;}");
    ui->noteUpdatedAt->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"));
    
    //this->setStyleSheet("OneNote{background-color: #bcd4bc;}");

    QTimer::singleShot(300, this, [this]() {
        setColor(m_color);
        //ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(221, 222, 144); border-radius: 10px;}");
        //this->setStyleSheet("OneNote{background-color: rgb(221, 222, 144)}");
    });
}

//изменения цветов
void OneNote::colorChangeYellow() {
    emit notesColorChanged(m_id, "yellow");
    ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(255, 235, 133); border-radius: 10px;}");
    m_color = "yellow";
}

void OneNote::colorChangeBlue() {
    emit notesColorChanged(m_id, "blue");
    ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(148, 194, 243); border-radius: 10px;}");
    m_color = "blue";
}

void OneNote::colorChangeGreen() {
    emit notesColorChanged(m_id, "green");
    ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(183, 242, 243); border-radius: 10px;}");
    m_color = "green";
}

void OneNote::colorChangeOrange() {
    emit notesColorChanged(m_id, "orange");
    ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(255, 195, 143); border-radius: 10px;}");
    m_color = "orange";
}

void OneNote::colorChangePink() {
    emit notesColorChanged(m_id, "pink");
    ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(255, 183, 208); border-radius: 10px;}");
    m_color = "pink";
}


void OneNote::setColor(const QString& color) {
    if (color == "pink") {
        ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(255, 183, 208); border-radius: 10px;}");
    } 
    else if (color == "green") {
        ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(183, 242, 243); border-radius: 10px;}");
    }
    else if (color == "orange") {
        ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(255, 195, 143); border-radius: 10px;}");
    }
    else if (color == "blue") {
        ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(148, 194, 243); border-radius: 10px;}");
    } 
    else {
        ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(255, 235, 133); border-radius: 10px;}");
    }
}

void OneNote::onDeleteClicked() {
    emit noteDeleted(m_id);

    ui->frameBgColor->setStyleSheet("QFrame{background-color: #e0baab;}");
    //this->setStyleSheet("OneNote{background-color: #e0baab;}");

    QTimer::singleShot(300, this, [this]() {
        ui->frameBgColor->setStyleSheet("QFrame{background-color: rgb(221, 222, 144);}");
        //this->setStyleSheet("OneNote{background-color: rgb(221, 222, 144)}");
        //emit noteDeleted(m_id);
        });
}


int OneNote::getId() const {
    return m_id;
}
QString OneNote::getContent() const{
    return ui->noteContent->toPlainText();
}

QString OneNote::getTitle() const {
    return  ui->noteTitle->text();
}

QDateTime OneNote::getCreatedAt() const {
    return  m_createdAt;
}

QDateTime OneNote::getupdatedAt()const {
    return  m_updatedAt;
}

