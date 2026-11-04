#include "hasherpage.h"
#include "ui_hasherpage.h"

HasherPage::HasherPage(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::HasherPage())
{
	ui->setupUi(this);

    setStyleSheet(R"(
        QCheckBox {
            border: none;
            background-color: transparent;
            
        }
        QCheckBox::indicator {
            width: 20px;
            height: 20px;
            border: 2px solid #d0b0eb;
            border-radius: 2px;
            background-color: #f1e0ff;
        }
        QCheckBox::indicator:checked {
            border-color: #647d6f;
            background-color: #ceeddc;
        }

        QRadioButton {
            border: none;
            background-color: transparent;
            spacing: 8px;
        }

        QRadioButton::indicator {
            width: 24px;  /* чуть больше для радио, т.к. круглому нужно больше места */
            height: 24px;
    
            /* Радио - это КРУГ, а не квадрат */
            border: 2px solid #d0b0eb;
            border-radius: 9px;  /* половина от (width + border) */
            background-color: #f1e0ff;
        }

        QRadioButton::indicator:checked {
            border-color: #647d6f;
            background-color: #ceeddc;
        }
    )");

    connect(ui->pushButtonMake, &QPushButton::clicked,
        this, &HasherPage::onMakeOperationPressed);

    connect(ui->pushButtonClean, &QPushButton::clicked,
        this, &HasherPage::onCleanPressed);
}

HasherPage::~HasherPage()
{
	delete ui;
}

void HasherPage::onMakeOperationPressed() {
    auto inputText = ui->plainTextEdit->toPlainText();
    if (inputText.trimmed().isEmpty()) {
        return;
    }

    QByteArray inputData = inputText.toUtf8();

    if (ui->radioButtonMD4->isChecked()) {
        QByteArray hash = QCryptographicHash::hash(inputData, QCryptographicHash::Md4);
        QString hashString = hash.toHex();
        ui->plainTextEditRes->setPlainText(hashString);
    }else if (ui->radioButtonMD5->isChecked()) {
        QByteArray hash = QCryptographicHash::hash(inputData, QCryptographicHash::Md5);
        QString hashString = hash.toHex();
        ui->plainTextEditRes->setPlainText(hashString);
    } else if (ui->radioButtonSHA1->isChecked()) {
        QByteArray hash = QCryptographicHash::hash(inputData, QCryptographicHash::Sha1);
        QString hashString = hash.toHex();
        ui->plainTextEditRes->setPlainText(hashString);
    } else if (ui->radioButtonSHA256->isChecked()) {
        QByteArray hash = QCryptographicHash::hash(inputData, QCryptographicHash::Sha256);
        QString hashString = hash.toHex();
        ui->plainTextEditRes->setPlainText(hashString);
    }else if (ui->radioButtonSHA512->isChecked()) {
        QByteArray hash = QCryptographicHash::hash(inputData, QCryptographicHash::Sha512);
        QString hashString = hash.toHex();
        ui->plainTextEditRes->setPlainText(hashString);
    }else if (ui->radioButtonBlake2b_256->isChecked()) {
        QByteArray hash = QCryptographicHash::hash(inputData, QCryptographicHash::Blake2b_256);
        QString hashString = hash.toHex();
        ui->plainTextEditRes->setPlainText(hashString);
    }else if (ui->radioButtonBlake2s_256->isChecked()) {
        QByteArray hash = QCryptographicHash::hash(inputData, QCryptographicHash::Blake2s_256);
        QString hashString = hash.toHex();
        ui->plainTextEditRes->setPlainText(hashString);
    }
}

void HasherPage::onCleanPressed() {
}


