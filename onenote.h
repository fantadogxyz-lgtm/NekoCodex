#ifndef ONENOTE_H
#define ONENOTE_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class OneNote;
}

class OneNote : public QWidget
{
    Q_OBJECT

public:
    explicit OneNote(QWidget *parent = nullptr);
    ~OneNote();

    void setData(int id, const QString& title,
        const QString& content,
        const QDateTime& createdAt,
        const QDateTime& updatedAt,
        const QString& color);

    int getId() const;
    QString getTitle() const;
    QString getContent() const;
    QDateTime getCreatedAt() const;
    QDateTime getupdatedAt() const;

    QTimer* m_autoSavingTimer;

    void setColor(const QString& color);


signals:
    void noteChanged(int id, const QString& title, const QString& content);
    void notesColorChanged(int id, const QString& color);
    void noteDeleted(int id);

private slots:
    void onDeleteClicked();
    void onAutoSave();
    void colorChangeYellow();
    void colorChangeBlue();
    void colorChangePink();
    void colorChangeOrange();
    void colorChangeGreen();

private:
    Ui::OneNote *ui;


    //данные одной заметки
    int m_id;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
    QString m_color;

};

#endif // ONENOTE_H
