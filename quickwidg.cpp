#include "quickwidg.h"
#include "ui_quickwidg.h"

QuickWidg::QuickWidg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QuickWidg)
{
    ui->setupUi(this);


    setWindowFlags(Qt::Dialog | Qt::Window);

    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->pushButtonEdit, &QPushButton::clicked,
        this, &QuickWidg::onEdinButtonPressed);

    connect(ui->pushButton_1, &QPushButton::clicked,
        this, &QuickWidg::onPushButton_1Pressed);
    connect(ui->pushButton_2, &QPushButton::clicked,
        this, &QuickWidg::onPushButton_2Pressed);
    connect(ui->pushButton_3, &QPushButton::clicked,
        this, &QuickWidg::onPushButton_3Pressed);
    connect(ui->pushButton_4, &QPushButton::clicked,
        this, &QuickWidg::onPushButton_4Pressed);
    connect(ui->pushButton_5, &QPushButton::clicked,
        this, &QuickWidg::onPushButton_5Pressed);
    connect(ui->pushButton_6, &QPushButton::clicked,
        this, &QuickWidg::onPushButton_6Pressed);
    connect(ui->pushButton_7, &QPushButton::clicked,
        this, &QuickWidg::onPushButton_7Pressed);
    connect(ui->pushButton_8, &QPushButton::clicked,
        this, &QuickWidg::onPushButton_8Pressed);
    connect(ui->pushButton_9, &QPushButton::clicked,
        this, &QuickWidg::onPushButton_9Pressed);

    setData();
   
}

QuickWidg::~QuickWidg()
{
    delete ui;
}

void QuickWidg::onEdinButtonPressed() {
    if (!settings) {
        settings = new QuickWidgSettings();
        setWindowFlags(Qt::Dialog | Qt::Window);
        connect(settings, &QObject::destroyed, this, &QuickWidg::onSettingsDestroyed);
        settings->setAttribute(Qt::WA_DeleteOnClose);
    }
    settings->show();
    this->hide();
    
}

void QuickWidg::setData()
{
    for (int i = 0; i < 9; i++) {
        filePatch[i] = Neko_settings::instance()->getShortcutPath(i);
        fileName[i] = Neko_settings::instance()->getShortcutText(i);
    }

    ui->pushButton_1->setText(fileName[0]);
    ui->pushButton_2->setText(fileName[1]);
    ui->pushButton_3->setText(fileName[2]);
    ui->pushButton_4->setText(fileName[3]);
    ui->pushButton_5->setText(fileName[4]);
    ui->pushButton_6->setText(fileName[5]);
    ui->pushButton_7->setText(fileName[6]);
    ui->pushButton_8->setText(fileName[7]);
    ui->pushButton_9->setText(fileName[8]);
}

void QuickWidg::onSettingsDestroyed() {
    settings = nullptr;
    this->close();
}

void QuickWidg::onPushButton_1Pressed() {
    if (filePatch[0].isEmpty() || filePatch[0] == "Empty") {
        qDebug() << "File is emty";
        return;
    }


    if (filePatch[0].endsWith(".exe", Qt::CaseInsensitive)) {
        bool startSuccess = QProcess::startDetached(filePatch[0]);

        if (!startSuccess) {
            qDebug() << "F: " << filePatch[0];
        }
    }
    else {
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePatch[0]));
        if (!success) {
            qDebug() << "FFF";
        }
    }
}

void QuickWidg::onPushButton_2Pressed() {
    if (filePatch[1].isEmpty() || filePatch[1] == "Empty") {
        qDebug() << "File is emty";
        return;
    }


    if (filePatch[1].endsWith(".exe", Qt::CaseInsensitive)) {
        bool startSuccess = QProcess::startDetached(filePatch[1]);

        if (!startSuccess) {
            qDebug() << "F: " << filePatch[1];
        }
    }
    else {
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePatch[1]));
        if (!success) {
            qDebug() << "FFF";
        }
    }
}
void QuickWidg::onPushButton_3Pressed() {
    if (filePatch[2].isEmpty() || filePatch[2] == "Empty") {
        qDebug() << "File is emty";
        return;
    }


    if (filePatch[2].endsWith(".exe", Qt::CaseInsensitive)) {
        bool startSuccess = QProcess::startDetached(filePatch[2]);

        if (!startSuccess) {
            qDebug() << "F: " << filePatch[2];
        }
    }
    else {
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePatch[2]));
        if (!success) {
            qDebug() << "FFF";
        }
    }
}
void QuickWidg::onPushButton_4Pressed() {
    if (filePatch[3].isEmpty() || filePatch[3] == "Empty") {
        qDebug() << "File is emty";
        return;
    }


    if (filePatch[3].endsWith(".exe", Qt::CaseInsensitive)) {
        bool startSuccess = QProcess::startDetached(filePatch[3]);

        if (!startSuccess) {
            qDebug() << "F: " << filePatch[3];
        }
    }
    else {
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePatch[3]));
        if (!success) {
            qDebug() << "FFF";
        }
    }
}
void QuickWidg::onPushButton_5Pressed() {
    if (filePatch[4].isEmpty() || filePatch[4] == "Empty") {
        qDebug() << "File is emty";
        return;
    }


    if (filePatch[4].endsWith(".exe", Qt::CaseInsensitive)) {
        bool startSuccess = QProcess::startDetached(filePatch[4]);

        if (!startSuccess) {
            qDebug() << "F: " << filePatch[4];
        }
    }
    else {
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePatch[4]));
        if (!success) {
            qDebug() << "FFF";
        }
    }
}
void QuickWidg::onPushButton_6Pressed() {
    if (filePatch[5].isEmpty() || filePatch[5] == "Empty") {
        qDebug() << "File is emty";
        return;
    }


    if (filePatch[5].endsWith(".exe", Qt::CaseInsensitive)) {
        bool startSuccess = QProcess::startDetached(filePatch[5]);

        if (!startSuccess) {
            qDebug() << "F: " << filePatch[5];
        }
    }
    else {
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePatch[5]));
        if (!success) {
            qDebug() << "FFF";
        }
    }
}
void QuickWidg::onPushButton_7Pressed() {
    if (filePatch[6].isEmpty() || filePatch[6] == "Empty") {
        qDebug() << "File is emty";
        return;
    }


    if (filePatch[6].endsWith(".exe", Qt::CaseInsensitive)) {
        bool startSuccess = QProcess::startDetached(filePatch[6]);

        if (!startSuccess) {
            qDebug() << "F: " << filePatch[6];
        }
    }
    else {
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePatch[6]));
        if (!success) {
            qDebug() << "FFF";
        }
    }
}
void QuickWidg::onPushButton_8Pressed() {
    if (filePatch[7].isEmpty() || filePatch[7] == "Empty") {
        qDebug() << "File is emty";
        return;
    }


    if (filePatch[7].endsWith(".exe", Qt::CaseInsensitive)) {
        bool startSuccess = QProcess::startDetached(filePatch[7]);

        if (!startSuccess) {
            qDebug() << "F: " << filePatch[7];
        }
    }
    else {
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePatch[7]));
        if (!success) {
            qDebug() << "FFF";
        }
    }
}
void QuickWidg::onPushButton_9Pressed() {
    if (filePatch[8].isEmpty() || filePatch[8] == "Empty") {
        qDebug() << "File is emty";
        return;
    }


    if (filePatch[8].endsWith(".exe", Qt::CaseInsensitive)) {
        bool startSuccess = QProcess::startDetached(filePatch[8]);

        if (!startSuccess) {
            qDebug() << "F: " << filePatch[8];
        }
    }
    else {
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePatch[8]));
        if (!success) {
            qDebug() << "FFF";
        }
    }
}
