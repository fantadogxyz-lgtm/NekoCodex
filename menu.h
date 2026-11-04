#ifndef MENU_H
#define MENU_H

#include <QDialog>
#include "notes.h"
#include "Events.h"
#include "datagenerator.h"
#include "githelper.h"
#include "encriptorpage.h"
#include "hasherpage.h"
#include "timerpage.h"
#include "jsongenerator.h"
#include "Settinds.h"
#include "AboutProgram.h"
//#include "events.h"


namespace Ui {
class Menu;
}

class Menu : public QDialog
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private slots:
    void on_pushButtonNotes_clicked();
    void on_pushButtonEvents_clicked();
    void on_pushButtonSimlpeDataGenerator_clicked();
    void on_pushButtonGitHelper_clicked();
    void on_pushButtonEncript_clicked();
    void on_pushButtonHasher_clicked();
    void on_pushButtonTimer_clicked();
    void on_pushButtonJsonGenerator_clicked();
    void on_pushButtonSettings_clicked();
    void on_pushButtonClose_clicked();
    void on_pushButtonAbout_clicked();



    public slots:
    void eventPassedSignalFromdragaablelabel(int id);
    void eventDeletedSignalFromdragaablelabel(int id);

signals:
    void eventPassedSignalToEvents(int id);
    void eventDeletedSignalToEvents(int id);
    void closeCodeka();

private:
    Ui::Menu *ui;
    Notes *notesWidg = nullptr;
    Events* eventsWidg = nullptr;
    DataGenerator* datagenerator = nullptr;
    GitHelper* githelper = nullptr;
    EncriptorPage* encrPage = nullptr;
    HasherPage* hashPage = nullptr;
    TimerPage* timerPage = nullptr;
    JsonGenerator* jsonGenerator = nullptr;
    Settinds* settings = nullptr;
    AboutProgram* aboutProgram = nullptr;
};

#endif // MENU_H
