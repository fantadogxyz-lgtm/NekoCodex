#ifndef NEKO_SETTINGS_H
#define NEKO_SETTINGS_H

#include <QSettings>

class Neko_settings
{
private:
    Neko_settings() = default;
    ~Neko_settings() = default;

    static QSettings& settings();

public:
    Neko_settings(Neko_settings& other) = delete;
    void operator=(Neko_settings other) = delete;

    static Neko_settings* instance();

    bool isCodekaFixedOnScreen();
    bool isCodekaInSlowMode();
    bool isRandomMessagesAllowed();
    int whatIsTheIntervalForRandomMessages();

    void setCodekaFixedOnScreen(bool value);
    void setCodekaInSlowMode(bool value);
    void setRandomMessagesAllowed(bool value);
    void setTheIntervalForRandomMessages(int intervalInMinutes);

    void resetToDefaults();


    QString getShortcutPath(int index);
    QString getShortcutText(int index);

    void setShortcutPath(int index, const QString& path);
    void setShortcutText(int index, const QString& text);
    void clearShortcut(int index);
};

#endif