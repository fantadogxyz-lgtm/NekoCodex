#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QPixmap>
#include <QScreen>
#include <QTimer>
#include <iostream>
#include <cstdlib>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    srand(static_cast<unsigned int>(time(nullptr)));

    animationTimer = nullptr;
    currentFrame = 0;
    isTurned = false;

    movementTimer = nullptr;
    moveDirection = 1;
    moveStep = 2;

    setFixedSize(160,160);
    qApp->setQuitOnLastWindowClosed(false);
    setAttribute(Qt::WA_TranslucentBackground); // окно с альфа-каналом
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    statusBar()->setSizeGripEnabled(false);
    ui->codeka->setAttribute(Qt::WA_TranslucentBackground);
    ui->codeka->setStyleSheet("background: transparent;");

    connect(this, &MainWindow::startEventCheker,
        this, &MainWindow::onStartEventCheker);

    connect(this, &MainWindow::StartTimersForTimePage,
        this, &MainWindow::onStartTimersForTimePage);

    connect(this, &MainWindow::StartTimersForReminders,
        this, &MainWindow::onStartTimersForReminders);

    connect(this, &MainWindow::eventPassedSignaltoDraggableLabel,
        ui->codeka, &DraggableLabel::eventPassedSignalFromMainWindow);

    connect(this, &MainWindow::eventDeletedSignaltoDraggableLabel,
        ui->codeka, &DraggableLabel::eventDeletedSignalFromMainWindow);

    connect(ui->codeka, &DraggableLabel::closeCodekaFromButtonToMainWindowSignal,
        this, &MainWindow::closeCodekaMainWindow);

    connect(ui->codeka, &DraggableLabel::labelPressed, this, &MainWindow::onNekoDragged);
    connect(ui->codeka, &DraggableLabel::labelReleased, this, &MainWindow::onNekoReleased);

    NekoBirth();

    //createTimerForCodekaActivity();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::NekoBirth(){
    pix = QPixmap(":/res/img/neko_sprites_on_tran_bg/fall.png");
    int w = ui->codeka->width();
    int h = ui->codeka->height();
    ui->codeka->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    move(screenGeometry.right()/2, 0);

    animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(2000);
    animation->setStartValue(pos());
    animation->setEndValue(QPoint(screenGeometry.right()/2, screenGeometry.bottom() - 160));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    QTimer::singleShot(100, this, [this]() { animation->start(); });

    connect(animation, &QPropertyAnimation::finished, this, [this](){
    pix = QPixmap(":/res/img/neko_sprites_on_tran_bg/get-up.png");
    ui->codeka->setPixmap(pix);

    QTimer::singleShot(1000, this, [this](){
        pix = QPixmap(":/res/img/neko_sprites_on_tran_bg/def.png");
        ui->codeka->setPixmap(pix);

        QTimer::singleShot(1000, this, [this]() {
            startNewActivity();
        });
        emit startEventCheker();
        emit StartTimersForTimePage();
        emit StartTimersForReminders();

        int time = Neko_settings::instance()->whatIsTheIntervalForRandomMessages();
        ReminderTimersManager::instance()->startCatFactTimer(time);

        });

    });


}


void MainWindow::startNewActivity()
{
    if (!Neko_settings::instance()->isCodekaInSlowMode()) {
        maxTime = 180000;
        minTime = 10000;
    }
    else {
        maxTime = 600000;
        minTime = 300000;
    }

    int activityTime = minTime + (rand() % (maxTime - minTime + 1));
    isTurned = (rand() % 2 == 0);

    // Выбор активности

     /**
        1 - ходить
        2 - играть в приставку
        3 - кушать
        4 - сидит в телефоне
        5 - играет на гитаре
        6 - спать
        7 - стоять
        8 - сидеть в ноуте
        9 - тоже ходить
    **/

    int whatActivity;
    if (Neko_settings::instance()->isCodekaFixedOnScreen()) {
        whatActivity = 2 + (rand() % (8 - 2 + 1));
    }
    else {
        whatActivity = 1 + (rand() % (9 - 1 + 1));
    }


    showActivity(whatActivity); 

    
    QTimer::singleShot(activityTime, this, [this]() {
        startNewActivity();  // через minutes новая активность
        });
}

void MainWindow::showActivity(int whatActivity)
{
    if (animationTimer) {
        animationTimer->stop();
        delete animationTimer;
        animationTimer = nullptr;
    }

    stopWalkingMovement();

    animationFrames.clear();

    switch (whatActivity) {
    case 1: // ходить
    case 9:
        animationFrames = {
            ":/res/img/neko_sprites_on_tran_bg/walk-1.png",
            ":/res/img/neko_sprites_on_tran_bg/walk-2.png",
            ":/res/img/neko_sprites_on_tran_bg/walk-3.png",
            ":/res/img/neko_sprites_on_tran_bg/walk-2.png"
        };

        if (moveDirection == -1)
            isTurned = true;
        else
            isTurned = false;

        startWalkingMovement();
        break;

    case 2: // играть в приставку
        animationFrames = {
            ":/res/img/neko_sprites_on_tran_bg/game-vr-1.png",
            ":/res/img/neko_sprites_on_tran_bg/game-vr-2.png"
        };
        break;

    case 3: // кушать
        animationFrames = {
            ":/res/img/neko_sprites_on_tran_bg/eat-1.png",
            ":/res/img/neko_sprites_on_tran_bg/eat-2.png"
        };
        break;

    case 4: // сидит в телефоне
        animationFrames = {
            ":/res/img/neko_sprites_on_tran_bg/tel-1.png",
            ":/res/img/neko_sprites_on_tran_bg/tel-2.png",
            ":/res/img/neko_sprites_on_tran_bg/tel-3.png",
            ":/res/img/neko_sprites_on_tran_bg/tel-4.png",
            ":/res/img/neko_sprites_on_tran_bg/tel-2.png",
            ":/res/img/neko_sprites_on_tran_bg/tel-1.png",
            ":/res/img/neko_sprites_on_tran_bg/tel-4.png",
            ":/res/img/neko_sprites_on_tran_bg/tel-3.png"
        };
        break;

    case 5: // играет на гитаре
        animationFrames = {
            ":/res/img/neko_sprites_on_tran_bg/gitar-1.png",
            ":/res/img/neko_sprites_on_tran_bg/gitar-2.png"
        };
        break;

    case 6: // спать
        animationFrames = {
            ":/res/img/neko_sprites_on_tran_bg/sleep-1.png",
            ":/res/img/neko_sprites_on_tran_bg/sleep-2.png",
            ":/res/img/neko_sprites_on_tran_bg/sleep-3.png",
            ":/res/img/neko_sprites_on_tran_bg/sleep-2.png",
            ":/res/img/neko_sprites_on_tran_bg/sleep-3.png",
            ":/res/img/neko_sprites_on_tran_bg/sleep-1.png",
            ":/res/img/neko_sprites_on_tran_bg/sleep-3.png"
        };
        break;

    case 7: // стоять
        animationFrames = {
            ":/res/img/neko_sprites_on_tran_bg/def.png",
            ":/res/img/neko_sprites_on_tran_bg/def-cloused-eyes.png"
        };
        /*
        *  animationFrames = {
           ":/res/img/new-codeka-sprites/codeka-def-1.png",
           ":/res/img/new-codeka-sprites/codeka-def-1.png",
           ":/res/img/new-codeka-sprites/codeka-def-2.png",
           ":/res/img/new-codeka-sprites/codeka-def-1.png",
           ":/res/img/new-codeka-sprites/codeka-def-3.png",
           ":/res/img/new-codeka-sprites/codeka-def-1.png",
           ":/res/img/new-codeka-sprites/codeka-def-1.png",
           ":/res/img/new-codeka-sprites/codeka-def-4.png"
        };
        */
       
        break;

    case 8: // сидеть в ноуте
        animationFrames = {
            ":/res/img/neko_sprites_on_tran_bg/comp-1.png",
            ":/res/img/neko_sprites_on_tran_bg/comp-2.png"
        };
        break;

    default:
        pix = QPixmap(":/res/img/neko_sprites_on_tran_bg/def.png");
        ui->codeka->setPixmap(pix.scaled(ui->codeka->width(),
            ui->codeka->height(),
            Qt::KeepAspectRatio));
        return;
    }

    currentFrame = 0;

    if (!animationFrames.isEmpty()) {
        pix = QPixmap(animationFrames[0]);
        QPixmap transformedPix;
        if (isTurned) {
            transformedPix = pix.transformed(QTransform().scale(-1, 1));
        }
        else {
            transformedPix = pix;
        }
        ui->codeka->setPixmap(transformedPix.scaled(ui->codeka->width(),
            ui->codeka->height(),
            Qt::KeepAspectRatio));
    }

    animationTimer = new QTimer(this);

    if (whatActivity != 1 && whatActivity != 9){
        auto changeFrame = [this]() {
        currentFrame = (currentFrame + 1) % animationFrames.size();

        pix = QPixmap(animationFrames[currentFrame]);
        QPixmap transformedPix;
        if (isTurned) {
            transformedPix = pix.transformed(QTransform().scale(-1, 1));
        }
        else {
            transformedPix = pix;
        }
        ui->codeka->setPixmap(transformedPix.scaled(ui->codeka->width(),
            ui->codeka->height(),
            Qt::KeepAspectRatio));

        int nextInterval = 40 + (rand() % 2001);
        animationTimer->setInterval(nextInterval);
        };

    connect(animationTimer, &QTimer::timeout, this, changeFrame);

    int firstInterval = 500 + (rand() % 1001);
    animationTimer->start(firstInterval);
    }
    else {
        auto changeFrame = [this]() {
            currentFrame = (currentFrame + 1) % animationFrames.size();

            pix = QPixmap(animationFrames[currentFrame]);
            QPixmap transformedPix;
            if (isTurned) {
                transformedPix = pix.transformed(QTransform().scale(-1, 1));
            }
            else {
                transformedPix = pix;
            }
            ui->codeka->setPixmap(transformedPix.scaled(ui->codeka->width(),
                ui->codeka->height(),
                Qt::KeepAspectRatio));

            int nextInterval = 500;
            animationTimer->setInterval(nextInterval);
            };

        connect(animationTimer, &QTimer::timeout, this, changeFrame);

        int firstInterval = 50;
        animationTimer->start(firstInterval);
    }
    
    

}

void MainWindow::startWalkingMovement()
{
    stopWalkingMovement();

    movementTimer = new QTimer(this);

    auto move = [this]() {
        QPoint currentPos = this->pos();

        QScreen* screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();

        int leftOffset = 20;     
        int rightOffset = 20;     

        int leftBorder = screenGeometry.left() + leftOffset;
        int rightBorder = screenGeometry.right() - this->width() - rightOffset;

        // Если дошли до левой границы + отступ
        if (currentPos.x() <= leftBorder && moveDirection == -1) {
            moveDirection = 1;    // разворачиваемся вправо
            isTurned = false;     // смотрит вправо

            if (!animationFrames.isEmpty()) {
                pix = QPixmap(animationFrames[currentFrame]);
                ui->codeka->setPixmap(pix.scaled(ui->codeka->width(),
                    ui->codeka->height(),
                    Qt::KeepAspectRatio));
            }
        }
        // Если дошли до правой границы + отступ
        else if (currentPos.x() >= rightBorder && moveDirection == 1) {
            moveDirection = -1;   // разворачиваемся влево
            isTurned = true;      // смотрит влево

            if (!animationFrames.isEmpty()) {
        pix = QPixmap(animationFrames[currentFrame]);
        ui->codeka->setPixmap(pix.transformed(QTransform().scale(-1, 1))
                              .scaled(ui->codeka->width(),
                                      ui->codeka->height(),
                                      Qt::KeepAspectRatio));
    }
        }

        int newX = currentPos.x() + (moveStep * moveDirection);
        this->move(newX, currentPos.y());
        };

    connect(movementTimer, &QTimer::timeout, this, move);

    movementTimer->start(50);

}

void MainWindow::stopWalkingMovement()
{
    if (movementTimer) {
        movementTimer->stop();
        delete movementTimer;
        movementTimer = nullptr;
    }
}



void MainWindow::changeCodekaActivity()
{
    /*
    if (!Neko_settings::instance()->isCodekaInSlowMode()) {
        maxTime = 300000;
        minTime = 60000;
    }
    else {
        maxTime = 600000;
        minTime = 300000;
    }

    int activityTime = minTime + (rand() % (maxTime - minTime + 1));

    int whatActivity;
    
        1 - ходить
        2 - играть в приставку
        3 - кушать
        4 - сидит в телефоне
        5 - играет на гитаре
        6 - спать
        7 - стоять
        8 - сидеть в ноуте
    

    if (Neko_settings::instance()->isCodekaFixedOnScreen()) {
        whatActivity = 2 + (rand() % (8 - 2 + 1));
    }
    else {
        whatActivity = 1 + (rand() % (8 - 1 + 1));
    }
    */

}

void MainWindow::onStartEventCheker() {
    if (!eventChecker) {
        eventChecker = EventChecker::instance(); 
    }
    eventChecker->initialize();

    connect(eventChecker, &EventChecker::eventPassed,
        this, &MainWindow::eventPassedFromEventChecker);

    connect(eventChecker, &EventChecker::eventDeleted,
        this, &MainWindow::eventDeletedFromEventChecker);
}

void MainWindow::eventPassedFromEventChecker(int id) {
    emit eventPassedSignaltoDraggableLabel(id);
}

void MainWindow::eventDeletedFromEventChecker(int id) {
    emit eventDeletedSignaltoDraggableLabel(id);
}


void MainWindow::onStartTimersForTimePage()
{
    if (!timersForTimerPage) {
        timersForTimerPage = TimersForTimerPage::instance();
        timersForTimerPage->initialize();
        qDebug() << "timers for Timer page: were created!";
    }
}

void MainWindow::onStartTimersForReminders()
{
    if (!reminderTimersManager) {
        reminderTimersManager = ReminderTimersManager::instance();
       reminderTimersManager->initialize();
       reminderTimersManager->startAllReminders();
        qDebug() << "timers for Reminders: were created!";
    }
}

void MainWindow::closeCodekaMainWindow() {
    qApp->quit();
}

void MainWindow::createTimerForCodekaActivity()
{
    /*
    if (!timerForCodekaActivity) {
        timerForCodekaActivity = new QTimer(this);
        timerForCodekaActivity->setInterval(1000);
        connect(timerForCodekaActivity, &QTimer::timeout,
            this, &MainWindow::changeCodekaActivity);
    }
    */



}


void MainWindow::onNekoDragged()
{
    qDebug() << "[Neko] fly";

    if (animationTimer && animationTimer->isActive()) {
        animationTimer->stop();
    }

    if (movementTimer && movementTimer->isActive()) {
        movementTimer->stop();
    }

    QPixmap pix123 = QPixmap(":/res/img/neko_sprites_on_tran_bg/fall.png");
    int w = ui->codeka->width();
    int h = ui->codeka->height();
    QPixmap transformedPix;
    if (isTurned) {
        transformedPix = pix123.transformed(QTransform().scale(-1, 1));
    }
    else {
        transformedPix = pix123;
    }

    ui->codeka->setPixmap(transformedPix.scaled(w, h, Qt::KeepAspectRatio));
}

void MainWindow::onNekoReleased()
{
    qDebug() << "[Neko] drop.";

    ui->codeka->setInteractionEnabled(false);

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    if (animation) { animation->deleteLater(); }
    animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(2000);
    animation->setStartValue(pos());
    animation->setEndValue(QPoint(this->x(), screenGeometry.bottom() - 160));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    QTimer::singleShot(50, this, [this]() { animation->start(); });

    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        ui->codeka->setInteractionEnabled(true);
        QPixmap transformedPix;
        if (isTurned) {
            transformedPix = pix.transformed(QTransform().scale(-1, 1));
        }
        else {
            transformedPix = pix;
        }


        int w = ui->codeka->width();
        int h = ui->codeka->height();
        ui->codeka->setPixmap(transformedPix.scaled(w, h, Qt::KeepAspectRatio));
        
            if (animationTimer && !animationTimer->isActive()) {
                animationTimer->start();
            }

            if (movementTimer && !movementTimer->isActive()) {
                movementTimer->start();
            }

           
           

       

        });
   
}



