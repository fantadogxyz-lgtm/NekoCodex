#pragma once

#include <QWidget>
#include "ui_oneevent.h"
#include "eventcreator.h"
#include <QPalette>


#include <QDateTime>


class oneevent : public QWidget
{
	Q_OBJECT

public:
	oneevent(QWidget *parent = nullptr);
	~oneevent();

    void setData(int id,
        const QString& content,
        const QDateTime& createdAt,
        const QDateTime& remindAt,
        bool isPassed);

    int getId() const;
    QString getContent() const;
    QDateTime getCreatedAt() const;
    QDateTime getRemindAt() const;
    bool getIsPassed() const;

private:
	Ui::oneevent *ui;


private slots:
   void onDeleteClicked();
   void onUpdateClicked();



private:
   int m_id;
   QString m_content;
   QDateTime m_createdAt;
   QDateTime m_remindAt;
   bool m_isPassed;

signals:
    void eventDeleted(int id);

    void updateRequested(int id, const QString& content,
        const QDateTime& createdAt,
        const QDateTime& remindAt,
        bool isPassed);

};
