#ifndef DRAGGABLELABEL_H
#define DRAGGABLELABEL_H

#pragma once
#include <QLabel>
#include <QMouseEvent>
#include <QPointer>
#include "menu.h"
#include "quickwidg.h"

class DraggableLabel : public QLabel {
    Q_OBJECT

public:
    explicit DraggableLabel(QWidget *parent = nullptr);
    void setInteractionEnabled(bool enabled) { m_isInteractionEnabled = enabled; }
   

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    

private:
    QPoint dragPos;

    Menu* m_menu = nullptr;
    QuickWidg* m_quickWid = nullptr;

    bool isDraggingNow = false;

    bool m_isInteractionEnabled = true;

public slots:
    void eventPassedSignalFromMainWindow(int id);

    void eventDeletedSignalFromMainWindow(int id);

    void closeCodekaFromButtonToMainWindowSlot(int id);

signals:
    void eventPassedSignaltoMenu(int id);
    void eventDeletedSignaltoMenu(int id);
    void closeCodekaFromButtonToMainWindowSignal();

    void labelPressed();
    void labelReleased(); 

private slots:
    void onMenuDestroyed();
    void onQuickWidgDestroyed();
};


#endif // DRAGGABLELABEL_H
