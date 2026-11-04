#include "menu.h"
#include "ui_menu.h"
#include "notes.h"



Menu::Menu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Menu)
{
    ui->setupUi(this);

    // Явно указываем, что это отдельное окно, но с родителем
    setWindowFlags(Qt::Dialog | Qt::Window);

    // Убираем возможность закрыть родителя, т е нашу кошкодевочку
    //setWindowModality(Qt::ApplicationModal);

    setAttribute(Qt::WA_DeleteOnClose);

    auto pixmap = QPixmap(":/res/img/menu_pics/neko_in_front_of_comp.png");
    ui->codekaPic->setPixmap(pixmap);
    ui->codekaPic->setScaledContents(true);

     setAttribute(Qt::WA_QuitOnClose, false);

     connect(ui->pushButtonSimpleDataGenerator, &QPushButton::clicked,
         this, &Menu::on_pushButtonSimlpeDataGenerator_clicked);

     connect(ui->pushButtonAboutGit, &QPushButton::clicked,
         this, &Menu::on_pushButtonGitHelper_clicked);

     connect(ui->pushButtonFastEncryption, &QPushButton::clicked,
         this, &Menu::on_pushButtonEncript_clicked);

     connect(ui->pushButtonFastHashing, &QPushButton::clicked,
         this, &Menu::on_pushButtonHasher_clicked);

     connect(ui->pushButtonTimer, &QPushButton::clicked,
         this, &Menu::on_pushButtonTimer_clicked);

     connect(ui->pushButtonSimpleJsonGenerator, &QPushButton::clicked,
         this, &Menu::on_pushButtonJsonGenerator_clicked);

     connect(ui->pushButtonSettings, &QPushButton::clicked,
         this, &Menu::on_pushButtonSettings_clicked);

     connect(ui->pushButtonCloseCodeka, &QPushButton::clicked,
         this, &Menu::on_pushButtonClose_clicked);

     connect(ui->pushButtonAbout, &QPushButton::clicked,
         this, &Menu::on_pushButtonAbout_clicked);

     ui->pushButtonNotes->setIconSize(QSize(100, 100));


     eventsWidg = new Events(this);
     ui->stackedWidget->addWidget(eventsWidg);

     // подключение сигналов 
     connect(this, &Menu::eventPassedSignalToEvents,
         eventsWidg, &Events::eventPassedSignalFromMenu);
     connect(this, &Menu::eventDeletedSignalToEvents,
         eventsWidg, &Events::eventDeletedSignalFromMenu);
}

Menu::~Menu()
{
    delete ui;
}


//ОТКРЫТЬ ВКЛАДКУ ЗАМЕТКИ
void Menu::on_pushButtonNotes_clicked()
{

    if (!notesWidg) {
        notesWidg = new Notes(this);
        ui->stackedWidget->addWidget(notesWidg); 
    }
    ui->stackedWidget->setCurrentWidget(notesWidg);
}

//ОТКРЫТЬ ВКЛАДКУ СОБЫТИЯ
void Menu::on_pushButtonEvents_clicked()
{
   if (!eventsWidg) {
        eventsWidg = new Events(this);
        ui->stackedWidget->addWidget(eventsWidg);
        connect(this, &Menu::eventPassedSignalToEvents,
            eventsWidg, &Events::eventPassedSignalFromMenu);

        connect(this, &Menu::eventDeletedSignalToEvents,
            eventsWidg, &Events::eventDeletedSignalFromMenu);
    }
    ui->stackedWidget->setCurrentWidget(eventsWidg);
}

void Menu::eventPassedSignalFromdragaablelabel(int id) {
    emit eventPassedSignalToEvents(id);
}

void Menu::eventDeletedSignalFromdragaablelabel(int id) {
    emit eventDeletedSignalToEvents(id);
}

void Menu::on_pushButtonSimlpeDataGenerator_clicked() {
    if (!datagenerator) {
        datagenerator = new DataGenerator(this);
        ui->stackedWidget->addWidget(datagenerator);
    }
    ui->stackedWidget->setCurrentWidget(datagenerator);
}

void Menu::on_pushButtonGitHelper_clicked()
{

    if (!githelper) {
        githelper = new GitHelper(this);
        ui->stackedWidget->addWidget(githelper);
    }
    ui->stackedWidget->setCurrentWidget(githelper);
}

void Menu::on_pushButtonEncript_clicked()
{

    if (!encrPage) {
        encrPage = new EncriptorPage(this);
        ui->stackedWidget->addWidget(encrPage);
    }
    ui->stackedWidget->setCurrentWidget(encrPage);
}

void Menu::on_pushButtonHasher_clicked()
{
    if (!hashPage) {
        hashPage = new HasherPage(this);
        ui->stackedWidget->addWidget(hashPage);
    }
    ui->stackedWidget->setCurrentWidget(hashPage);
}


void Menu::on_pushButtonTimer_clicked()
{
    if (!timerPage) {
        timerPage = new TimerPage(this);
        ui->stackedWidget->addWidget(timerPage);
    }
    ui->stackedWidget->setCurrentWidget(timerPage);
}

void Menu::on_pushButtonJsonGenerator_clicked()
{
    if (!jsonGenerator) {
        jsonGenerator = new JsonGenerator(this);
        ui->stackedWidget->addWidget(jsonGenerator);
    }
    ui->stackedWidget->setCurrentWidget(jsonGenerator);
}

void Menu::on_pushButtonSettings_clicked()
{
    if (!settings) {
        settings = new Settinds(this);
        ui->stackedWidget->addWidget(settings);
    }
    ui->stackedWidget->setCurrentWidget(settings);
}

void Menu::on_pushButtonAbout_clicked()
{
    if (!aboutProgram) {
        aboutProgram = new AboutProgram();
        aboutProgram->setAttribute(Qt::WA_DeleteOnClose);
        aboutProgram->setAttribute(Qt::WA_QuitOnClose, false);
        //aboutProgram->setWindowFlags(aboutProgram->windowFlags() | Qt::WindowStaysOnTopHint);
        aboutProgram->setFixedSize(aboutProgram->size());
        connect(aboutProgram, &QObject::destroyed, this, [this]() {
            qDebug() << "aboutProgram destroyed, nulling pointer";
            aboutProgram = nullptr;
            });
    }
    aboutProgram->show();
}

void Menu::on_pushButtonClose_clicked()
{
    emit closeCodeka();
}
