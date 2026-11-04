#include "draggablelabel.h"
#include "ui_mainwindow.h"
#include "menu.h"


DraggableLabel::DraggableLabel(QWidget *parent) : QLabel(parent) {
    isDraggingNow = false;
}

void DraggableLabel::mousePressEvent(QMouseEvent *event){
    if (!m_isInteractionEnabled) return; // Блок

    if (event->button() == Qt::LeftButton) {
        dragPos = event->globalPosition().toPoint();
        isDraggingNow = false; 
    }
}


void DraggableLabel::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (isDraggingNow) {
            isDraggingNow = false;
            emit labelReleased(); 
        }
    }
    QLabel::mouseReleaseEvent(event);
}
void DraggableLabel::mouseMoveEvent(QMouseEvent *event){
    if (!m_isInteractionEnabled) return; // Блок
    if (event->buttons() & Qt::LeftButton) {
        if (!isDraggingNow) {
            isDraggingNow = true;
            emit labelPressed();
        }
        QWidget* w = window();
        QPoint diff = event->globalPosition().toPoint() - dragPos;
        w->move(w->x() + diff.x(), w->y() + diff.y());
        dragPos = event->globalPosition().toPoint();
    }
}

void DraggableLabel::mouseDoubleClickEvent(QMouseEvent *event){
    if (!m_isInteractionEnabled) return; // Блок
    if (event->button() == Qt::LeftButton){
        if (!m_menu) {
            m_menu = new Menu(nullptr);
            m_menu->setAttribute(Qt::WA_DeleteOnClose);
            //m_menu->setWindowTitle("Menu - Neko's cute house <3");
            //m_menu->setModal(true);  // делает модальным, т е нельзя будет кликнуть на кодеку, пока менюшка не закроется



            connect(m_menu, &QObject::destroyed, this, &DraggableLabel::onMenuDestroyed);

            connect(this, &DraggableLabel::eventPassedSignaltoMenu,
                m_menu, &Menu::eventPassedSignalFromdragaablelabel);

            connect(this, &DraggableLabel::eventDeletedSignaltoMenu,
                m_menu, &Menu::eventDeletedSignalFromdragaablelabel);

            connect(m_menu, &Menu::closeCodeka,
                this, &DraggableLabel::closeCodekaFromButtonToMainWindowSignal);


        }
        m_menu->show();    
        m_menu->activateWindow();
    } else if (event->button() == Qt::RightButton){
        if (!m_quickWid) {
            m_quickWid = new QuickWidg();
            m_quickWid->setWindowTitle("Быстрые кошко-ярлычки");
            m_quickWid->setAttribute(Qt::WA_DeleteOnClose);

            connect(m_quickWid, &QObject::destroyed, this, &DraggableLabel::onQuickWidgDestroyed);

            m_quickWid->show();
            m_quickWid->activateWindow();
        }

    }
    


}

void DraggableLabel::onMenuDestroyed() {
    m_menu = nullptr;
}

void DraggableLabel::onQuickWidgDestroyed() {
    m_quickWid = nullptr;
}

void DraggableLabel::eventPassedSignalFromMainWindow(int id) {
    emit eventPassedSignaltoMenu(id);
}

void DraggableLabel::eventDeletedSignalFromMainWindow(int id) {
    emit eventDeletedSignaltoMenu(id);
}

void DraggableLabel::closeCodekaFromButtonToMainWindowSlot(int id)
{
    emit closeCodekaFromButtonToMainWindowSignal();
}

