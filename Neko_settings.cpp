#include "Neko_settings.h"
#include <QCoreApplication>
#include <QDir>

static const QString COMPANY_NAME = "NekoCodex";
static const QString APP_NAME = "NekoCodex";

Neko_settings* Neko_settings::instance() {
    static Neko_settings instance;
    return &instance;
}

QSettings& Neko_settings::settings() {
    static QSettings settings(COMPANY_NAME, APP_NAME);
    return settings;
}

bool Neko_settings::isCodekaFixedOnScreen() {
    return settings().value("CodekaFixedOnScreen", false).toBool();
}

bool Neko_settings::isCodekaInSlowMode() {
    return settings().value("CodekaInSlowMode", false).toBool();
}

bool Neko_settings::isRandomMessagesAllowed() {
    return settings().value("RandomMessagesAllowed", true).toBool();
}

int Neko_settings::whatIsTheIntervalForRandomMessages() //in minutes
{
    return settings().value("intervalForRandomMessages", 30).toInt();
}

void Neko_settings::setCodekaFixedOnScreen(bool value) {
    settings().setValue("CodekaFixedOnScreen", value);
    settings().sync();
}

void Neko_settings::setCodekaInSlowMode(bool value) {
    settings().setValue("CodekaInSlowMode", value);
    settings().sync();
}

void Neko_settings::setRandomMessagesAllowed(bool value) {
    settings().setValue("RandomMessagesAllowed", value);
    settings().sync();
}

void Neko_settings::setTheIntervalForRandomMessages(int intervalInMinutes)
{
    settings().setValue("intervalForRandomMessages", intervalInMinutes);
    settings().sync();
}

void Neko_settings::resetToDefaults() {
    settings().clear();
    setCodekaFixedOnScreen(false);
    setCodekaInSlowMode(false);
    setRandomMessagesAllowed(true);
    setTheIntervalForRandomMessages(30);

    for (int i = 0; i < 9; ++i) {
        clearShortcut(i);
    }
}


QString Neko_settings::getShortcutPath(int index) {
    return settings().value(QString("Shortcut_%1/path").arg(index), "Empty").toString();
}

QString Neko_settings::getShortcutText(int index) {
    return settings().value(QString("Shortcut_%1/text").arg(index), "").toString();
}

void Neko_settings::setShortcutPath(int index, const QString& path) {
    QString preparedPath = path.isEmpty() ? "Empty" : path;
    settings().setValue(QString("Shortcut_%1/path").arg(index), preparedPath);
    settings().sync(); // Принудительно сохраняем в ОС
}

void Neko_settings::setShortcutText(int index, const QString& text) {
    settings().setValue(QString("Shortcut_%1/text").arg(index), text);
    settings().sync();
}

void Neko_settings::clearShortcut(int index) {
    settings().setValue(QString("Shortcut_%1/path").arg(index), "Empty");
    settings().setValue(QString("Shortcut_%1/text").arg(index), "");
    settings().sync();
}