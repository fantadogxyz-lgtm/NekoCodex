#ifndef QUICKWIDG_H
#define QUICKWIDG_H

#include <QWidget>
#include "QuickWidgSettings.h"
#include "Neko_settings.h"

namespace Ui {
class QuickWidg;
}

class QuickWidg : public QWidget
{
    Q_OBJECT

public:
    explicit QuickWidg(QWidget *parent = nullptr);
    ~QuickWidg();

private:
    Ui::QuickWidg *ui;

    QuickWidgSettings* settings = nullptr;
    QString filePatch[9];
    QString fileName[9];
    void setData();

private slots:
    void onEdinButtonPressed();
    void onSettingsDestroyed();

    void onPushButton_1Pressed();
    void onPushButton_2Pressed();
    void onPushButton_3Pressed();
    void onPushButton_4Pressed();
    void onPushButton_5Pressed();
    void onPushButton_6Pressed();
    void onPushButton_7Pressed();
    void onPushButton_8Pressed();
    void onPushButton_9Pressed();
};

#endif // QUICKWIDG_H
