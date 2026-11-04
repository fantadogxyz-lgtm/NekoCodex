#ifndef NOTES_H
#define NOTES_H

#include <QWidget>
#include <QGridLayout>
#include <QVector>
#include "DatabaseNoteManager.h"
#include "OneNote.h"
#include <algorithm>
#include <QEvent>

namespace Ui {
class Notes;
}

class Notes : public QWidget
{
    Q_OBJECT

public:
    explicit Notes(QWidget *parent = nullptr);
    ~Notes();

    void loadNotes();
    void cleanNotes();

private slots:
    void onNoteDeleted(int id);
    void onNoteChanged(int id, const QString& title, const QString& content);
    void onNoteColorChanged(int id,const QString& color);
    void onCreateNoteClicked();
    
protected:
    void showEvent(QShowEvent* event) override;



private:
    void arrangeNotes();
    void setupGrid();

    QVector<OneNote*> m_notes;

    QGridLayout* m_gridLayout;

    Ui::Notes *ui;

    static constexpr int NOTE_WIDTH = 350;
    static constexpr int NOTE_HEIGHT = 450;
    static constexpr int COLUMNS = 2;
    bool m_firstShow;
    
};

#endif // NOTES_H
