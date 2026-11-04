#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QPointer>
#include "menu.h"
#include "EventChecker.h"
#include "TimersForTimerPage.h"
#include "Neko_settings.h"
#include <QTimer>
#include <QStringList>
#include "ReminderTimersManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPixmap pix;
    Ui::MainWindow *ui;
    void NekoBirth();
    QPropertyAnimation* animation;
    QTimer* timerForCodekaActivity;


    void createTimerForCodekaActivity();

    void changeCodekaActivity();

    void startNewActivity();

    void showActivity(int id);

    
    void startWalkingMovement();
    void stopWalkingMovement();

    QTimer* animationTimer;
    int currentFrame;
    bool isTurned;
    QStringList animationFrames;

    QTimer* movementTimer;
    int moveDirection;
    int moveStep;

    int maxTime;
    int minTime;

   
    
signals:
    void startEventCheker();
    void StartTimersForTimePage();
    void StartTimersForReminders();
    void eventPassedSignaltoDraggableLabel(int id);
    void eventDeletedSignaltoDraggableLabel(int id);

private slots:
    void onStartEventCheker();
    void onStartTimersForTimePage();
    void onStartTimersForReminders();
    void eventPassedFromEventChecker(int id);
    void eventDeletedFromEventChecker(int id);

    void onNekoDragged();  
    void onNekoReleased(); 


public slots:
    void closeCodekaMainWindow();
private:
    EventChecker* eventChecker = nullptr;
    TimersForTimerPage* timersForTimerPage = nullptr;
    ReminderTimersManager* reminderTimersManager = nullptr;
};
#endif // MAINWINDOW_H
