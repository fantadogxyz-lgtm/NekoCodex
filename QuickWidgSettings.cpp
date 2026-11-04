#include "QuickWidgSettings.h"
#include "ui_QuickWidgSettings.h"
#include "Neko_settings.h"
#include <QFileDialog>
#include <QDebug>

QuickWidgSettings::QuickWidgSettings(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::QuickWidgSettings())
{
    ui->setupUi(this);
    loadSettingsToUI();
    connect(ui->BrowseButton_1, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonBrowse_1_clicked);
    connect(ui->BrowseButton_2, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonBrowse_2_clicked);
    connect(ui->BrowseButton_3, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonBrowse_3_clicked);
    connect(ui->BrowseButton_4, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonBrowse_4_clicked);
    connect(ui->BrowseButton_5, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonBrowse_5_clicked);
    connect(ui->BrowseButton_6, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonBrowse_6_clicked);
    connect(ui->BrowseButton_7, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonBrowse_7_clicked);
    connect(ui->BrowseButton_8, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonBrowse_8_clicked);
    connect(ui->BrowseButton_9, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonBrowse_9_clicked);

    connect(ui->CleanButton_1, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonClean_1_clicked);
    connect(ui->CleanButton_2, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonClean_2_clicked);
    connect(ui->CleanButton_3, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonClean_3_clicked);
    connect(ui->CleanButton_4, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonClean_4_clicked);
    connect(ui->CleanButton_5, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonClean_5_clicked);
    connect(ui->CleanButton_6, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonClean_6_clicked);
    connect(ui->CleanButton_7, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonClean_7_clicked);
    connect(ui->CleanButton_8, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonClean_8_clicked);
    connect(ui->CleanButton_9, &QPushButton::clicked, this, &QuickWidgSettings::on_pushButtonClean_9_clicked);

    m_autoSaveTimer = new QTimer(this);
    m_autoSaveTimer->setSingleShot(true);
    connect(m_autoSaveTimer, &QTimer::timeout, this, &QuickWidgSettings::triggerAutoSave);

    connect(ui->lineEditText_1, &QLineEdit::textChanged, this, &QuickWidgSettings::onAnyLineEditTextChanged);
    connect(ui->lineEditText_2, &QLineEdit::textChanged, this, &QuickWidgSettings::onAnyLineEditTextChanged);
    connect(ui->lineEditText_3, &QLineEdit::textChanged, this, &QuickWidgSettings::onAnyLineEditTextChanged);
    connect(ui->lineEditText_4, &QLineEdit::textChanged, this, &QuickWidgSettings::onAnyLineEditTextChanged);
    connect(ui->lineEditText_5, &QLineEdit::textChanged, this, &QuickWidgSettings::onAnyLineEditTextChanged);
    connect(ui->lineEditText_6, &QLineEdit::textChanged, this, &QuickWidgSettings::onAnyLineEditTextChanged);
    connect(ui->lineEditText_7, &QLineEdit::textChanged, this, &QuickWidgSettings::onAnyLineEditTextChanged);
    connect(ui->lineEditText_8, &QLineEdit::textChanged, this, &QuickWidgSettings::onAnyLineEditTextChanged);
    connect(ui->lineEditText_9, &QLineEdit::textChanged, this, &QuickWidgSettings::onAnyLineEditTextChanged);

 
}

QuickWidgSettings::~QuickWidgSettings()
{
    delete ui;
}

void QuickWidgSettings::on_pushButtonBrowse_1_clicked() {
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("choose file"), "C:/", QString("All (*.*);;Exe (*.exe)"));
    if (!selectedFile.isEmpty()) {
        m_filePath_1 = selectedFile;
        ui->lineEditFile_1->setText(selectedFile);
        Neko_settings::instance()->setShortcutPath(0, selectedFile);
        ui->lineEditText_1->setReadOnly(false);
        ui->lineEditText_1->setPlaceholderText("Enter name");
    }
}
void QuickWidgSettings::on_pushButtonBrowse_2_clicked() {
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("choose file"), "C:/", QString("All (*.*);;Exe (*.exe)"));
    if (!selectedFile.isEmpty()) {
        m_filePath_2 = selectedFile;
        ui->lineEditFile_2->setText(selectedFile);
        Neko_settings::instance()->setShortcutPath(1, selectedFile);
        ui->lineEditText_2->setReadOnly(false);
        ui->lineEditText_2->setPlaceholderText("Enter name");
    }
}
void QuickWidgSettings::on_pushButtonBrowse_3_clicked() {
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("choose file"), "C:/", QString("All (*.*);;Exe (*.exe)"));
    if (!selectedFile.isEmpty()) {
        m_filePath_3 = selectedFile;
        ui->lineEditFile_3->setText(selectedFile);
        Neko_settings::instance()->setShortcutPath(2, selectedFile);
        ui->lineEditText_3->setReadOnly(false);
        ui->lineEditText_3->setPlaceholderText("Enter name");
    }
}
void QuickWidgSettings::on_pushButtonBrowse_4_clicked() {
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("choose file"), "C:/", QString("All (*.*);;Exe (*.exe)"));
    if (!selectedFile.isEmpty()) {
        m_filePath_4 = selectedFile;
        ui->lineEditFile_4->setText(selectedFile);
        Neko_settings::instance()->setShortcutPath(3, selectedFile);
        ui->lineEditText_4->setReadOnly(false);
        ui->lineEditText_4->setPlaceholderText("Enter name");
    }
}
void QuickWidgSettings::on_pushButtonBrowse_5_clicked() {
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("choose file"), "C:/", QString("All (*.*);;Exe (*.exe)"));
    if (!selectedFile.isEmpty()) {
        m_filePath_5 = selectedFile;
        ui->lineEditFile_5->setText(selectedFile);
        Neko_settings::instance()->setShortcutPath(4, selectedFile);
        ui->lineEditText_5->setReadOnly(false);
        ui->lineEditText_5->setPlaceholderText("Enter name");
    }
}
void QuickWidgSettings::on_pushButtonBrowse_6_clicked() {
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("choose file"), "C:/", QString("All (*.*);;Exe (*.exe)"));
    if (!selectedFile.isEmpty()) {
        m_filePath_6 = selectedFile;
        ui->lineEditFile_6->setText(selectedFile);
        Neko_settings::instance()->setShortcutPath(5, selectedFile);
        ui->lineEditText_6->setReadOnly(false);
        ui->lineEditText_6->setPlaceholderText("Enter name");
    }
}
void QuickWidgSettings::on_pushButtonBrowse_7_clicked() {
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("choose file"), "C:/", QString("All (*.*);;Exe (*.exe)"));
    if (!selectedFile.isEmpty()) {
        m_filePath_7 = selectedFile;
        ui->lineEditFile_7->setText(selectedFile);
        Neko_settings::instance()->setShortcutPath(6, selectedFile);
        ui->lineEditText_7->setReadOnly(false);
        ui->lineEditText_7->setPlaceholderText("Enter name");
    }
}
void QuickWidgSettings::on_pushButtonBrowse_8_clicked() {
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("choose file"), "C:/", QString("All (*.*);;Exe (*.exe)"));
    if (!selectedFile.isEmpty()) {
        m_filePath_8 = selectedFile;
        ui->lineEditFile_8->setText(selectedFile);
        Neko_settings::instance()->setShortcutPath(7, selectedFile);
        ui->lineEditText_8->setReadOnly(false);
        ui->lineEditText_8->setPlaceholderText("Enter name");
    }
}
void QuickWidgSettings::on_pushButtonBrowse_9_clicked() {
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("choose file"), "C:/", QString("All (*.*);;Exe (*.exe)"));
    if (!selectedFile.isEmpty()) {
        m_filePath_9 = selectedFile;
        ui->lineEditFile_9->setText(selectedFile);
        Neko_settings::instance()->setShortcutPath(8, selectedFile);
        ui->lineEditText_9->setReadOnly(false);
        ui->lineEditText_9->setPlaceholderText("Enter name");
    }
}

void QuickWidgSettings::triggerAutoSave()
{
    qDebug() << ".5 sec left";
    for (int i = 0; i < 9; ++i) {
        autoSaveData(i, m_filePaths[i]);
    }
}

void QuickWidgSettings::autoSaveData(int index, const QString& text)
{
    Neko_settings::instance()->setShortcutText(index, text);
}

void QuickWidgSettings::onAnyLineEditTextChanged(const QString& text)
{
    QLineEdit* currentLineEdit = qobject_cast<QLineEdit*>(sender());

    if (currentLineEdit) {
        int index = 0;
        if (currentLineEdit == ui->lineEditText_1) index = 0;
        else if (currentLineEdit == ui->lineEditText_2) index = 1;
        else if (currentLineEdit == ui->lineEditText_3) index = 2;
        else if (currentLineEdit == ui->lineEditText_4) index = 3;
        else if (currentLineEdit == ui->lineEditText_5) index = 4;
        else if (currentLineEdit == ui->lineEditText_6) index = 5;
        else if (currentLineEdit == ui->lineEditText_7) index = 6;
        else if (currentLineEdit == ui->lineEditText_8) index = 7;
        else if (currentLineEdit == ui->lineEditText_9) index = 8;

        m_filePaths[index] = text;
        qDebug() << "pole: " << (index + 1) << "change on: " << text;
    }

    m_autoSaveTimer->start(500);
}

void QuickWidgSettings::on_pushButtonClean_1_clicked() { cleanSlot(0); }
void QuickWidgSettings::on_pushButtonClean_2_clicked() { cleanSlot(1); }
void QuickWidgSettings::on_pushButtonClean_3_clicked() { cleanSlot(2); }
void QuickWidgSettings::on_pushButtonClean_4_clicked() { cleanSlot(3); }
void QuickWidgSettings::on_pushButtonClean_5_clicked() { cleanSlot(4); }
void QuickWidgSettings::on_pushButtonClean_6_clicked() { cleanSlot(5); }
void QuickWidgSettings::on_pushButtonClean_7_clicked() { cleanSlot(6); }
void QuickWidgSettings::on_pushButtonClean_8_clicked() { cleanSlot(7); }
void QuickWidgSettings::on_pushButtonClean_9_clicked() { cleanSlot(8); }

void QuickWidgSettings::cleanSlot(int index)
{
    Neko_settings::instance()->clearShortcut(index);

    switch (index) {
    case 0: m_filePath_1.clear(); ui->lineEditFile_1->setText("Empty"); ui->lineEditText_1->clear(); break;
    case 1: m_filePath_2.clear(); ui->lineEditFile_2->setText("Empty"); ui->lineEditText_2->clear(); break;
    case 2: m_filePath_3.clear(); ui->lineEditFile_3->setText("Empty"); ui->lineEditText_3->clear(); break;
    case 3: m_filePath_4.clear(); ui->lineEditFile_4->setText("Empty"); ui->lineEditText_4->clear(); break;
    case 4: m_filePath_5.clear(); ui->lineEditFile_5->setText("Empty"); ui->lineEditText_5->clear(); break;
    case 5: m_filePath_6.clear(); ui->lineEditFile_6->setText("Empty"); ui->lineEditText_6->clear(); break;
    case 6: m_filePath_7.clear(); ui->lineEditFile_7->setText("Empty"); ui->lineEditText_7->clear(); break;
    case 7: m_filePath_8.clear(); ui->lineEditFile_8->setText("Empty"); ui->lineEditText_8->clear(); break;
    case 8: m_filePath_9.clear(); ui->lineEditFile_9->setText("Empty"); ui->lineEditText_9->clear(); break;
    default: return;
    }

    m_filePaths[index].clear();

    QLineEdit* txtEdit = getLineEditButtonText(index);
    if (txtEdit) {
        txtEdit->setReadOnly(true);
        txtEdit->setPlaceholderText("Choose file");
    }

    qDebug() << "Slot " << (index + 1) << "is clean now!!!";
}

QLineEdit* QuickWidgSettings::getLineEditButtonText(int index) {
    switch (index) {
    case 0: return ui->lineEditText_1;
    case 1: return ui->lineEditText_2;
    case 2: return ui->lineEditText_3;
    case 3: return ui->lineEditText_4;
    case 4: return ui->lineEditText_5;
    case 5: return ui->lineEditText_6;
    case 6: return ui->lineEditText_7;
    case 7: return ui->lineEditText_8;
    case 8: return ui->lineEditText_9;
    default: return nullptr;
    }
}

void QuickWidgSettings::loadSettingsToUI()
{
    for (int i = 0; i < 9; ++i) {
        QString path = Neko_settings::instance()->getShortcutPath(i);
        QString text = Neko_settings::instance()->getShortcutText(i);

        m_filePaths[i] = text;

        switch (i) {
        case 0: m_filePath_1 = (path == "Empty") ? "" : path; break;
        case 1: m_filePath_2 = (path == "Empty") ? "" : path; break;
        case 2: m_filePath_3 = (path == "Empty") ? "" : path; break;
        case 3: m_filePath_4 = (path == "Empty") ? "" : path; break;
        case 4: m_filePath_5 = (path == "Empty") ? "" : path; break;
        case 5: m_filePath_6 = (path == "Empty") ? "" : path; break;
        case 6: m_filePath_7 = (path == "Empty") ? "" : path; break;
        case 7: m_filePath_8 = (path == "Empty") ? "" : path; break;
        case 8: m_filePath_9 = (path == "Empty") ? "" : path; break;
        }

        QLineEdit* fileEdit = findChild<QLineEdit*>(QString("lineEditFile_%1").arg(i + 1));
        QLineEdit* textEdit = findChild<QLineEdit*>(QString("lineEditText_%1").arg(i + 1));

        if (fileEdit && textEdit) {
            fileEdit->setText(path);
            textEdit->setText(text);
            fileEdit->setReadOnly(true);
            if (path == "Empty") {
                textEdit->setReadOnly(true);
                textEdit->setPlaceholderText("Choose file");
            }
            else {
                textEdit->setReadOnly(false);
                textEdit->setPlaceholderText("Enter name");
            }
        }
    }
}