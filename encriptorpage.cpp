#include "encriptorpage.h"
#include "ui_encriptorpage.h"

EncriptorPage::EncriptorPage(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::EncriptorPage())
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

  

    connect(ui->radioButtonAES128, &QRadioButton::toggled, 
        this, &EncriptorPage::onButtonAES128);
    connect(ui->radioButtonAES256, &QRadioButton::toggled, 
        this, &EncriptorPage::onButtonAES256);
    connect(ui->radioButtonBase64, &QRadioButton::toggled, 
        this, &EncriptorPage::onButtonBase64);
    connect(ui->radioButtonXOR, &QRadioButton::toggled, 
        this, &EncriptorPage::onButtonXOR);

    connect(ui->pushButtonClean, &QPushButton::clicked,
        this, &EncriptorPage::cleanTextOutputs);

    connect(ui->pushButtonMake, &QPushButton::clicked,
        this, &EncriptorPage::makeOperation);

    connect(ui->radioButtonShifrovanie, &QRadioButton::toggled,
        this, &EncriptorPage:: onChangeDecrypt);
    connect(ui->radioButtonDeshifrovanie, &QRadioButton::toggled,
        this, &EncriptorPage:: onChangeDecrypt);
}

EncriptorPage::~EncriptorPage()
{
	delete ui;
}

void EncriptorPage::hidePassword()
{
    ui->labelPassword->setVisible(false);
    ui->plainTextEditPassword->clear();
    ui->plainTextEditPassword->setVisible(false);
}

void EncriptorPage::showPassword()
{
    ui->labelPassword->setVisible(true);
    ui->plainTextEditPassword->setVisible(true);
}

void EncriptorPage::onButtonAES256()
{
    if (ui->radioButtonAES256->isChecked()) {
        showPassword();
        modeAES256 = true;
        modeAES128 = false;
        modeAESBase64 = false;
        modeXOR = false;
    }
    
}

void EncriptorPage::onButtonAES128()
{
    if (ui->radioButtonAES128->isChecked()) {
        showPassword();
        modeAES256 = false;
        modeAES128 = true;
        modeAESBase64 = false;
        modeXOR = false;
    }
}

void EncriptorPage::onButtonBase64()
{
    if (ui->radioButtonBase64->isChecked()) {
        hidePassword();
        modeAES256 = false;
        modeAES128 = false;
        modeAESBase64 = true;
        modeXOR = false;
    }
}

void EncriptorPage::onButtonXOR()
{
    if (ui->radioButtonXOR->isChecked()) {
        showPassword();
        modeAES256 = false;
        modeAES128 = false;
        modeAESBase64 = false;
        modeXOR = true;
    }
}

void EncriptorPage::cleanTextOutputs()
{
    ui->plainTextEditInput->clear();
    ui->plainTextEditOutput->clear();

    if(ui->plainTextEditPassword->isVisible() == true) ui->plainTextEditPassword->clear();
}

void EncriptorPage::makeOperation()
{
    if (modeAES256) {
        auto* cryptoHelper = CryptoHelper::instance();
        if (!decrypt) {
            ui->plainTextEditOutput->setPlainText(cryptoHelper->encryptAES256(
            ui->plainTextEditInput->toPlainText(), ui->plainTextEditPassword->toPlainText()));
        }
        else {
            ui->plainTextEditOutput->setPlainText(cryptoHelper->decryptAES256(
                ui->plainTextEditInput->toPlainText(), ui->plainTextEditPassword->toPlainText()));
        }
        
    } else if (modeAES128) {
        auto* cryptoHelper = CryptoHelper::instance();
        if (!decrypt) {
            ui->plainTextEditOutput->setPlainText(cryptoHelper->encryptAES128(
                ui->plainTextEditInput->toPlainText(), ui->plainTextEditPassword->toPlainText()));
        }
        else {
            ui->plainTextEditOutput->setPlainText(cryptoHelper->decryptAES128(
                ui->plainTextEditInput->toPlainText(), ui->plainTextEditPassword->toPlainText()));
        }

    }
    else if (modeAESBase64) {
        auto* cryptoHelper = CryptoHelper::instance();
        if (!decrypt) {
            ui->plainTextEditOutput->setPlainText(cryptoHelper->toBase64(
                ui->plainTextEditInput->toPlainText()));
        }
        else {
            ui->plainTextEditOutput->setPlainText(cryptoHelper->fromBase64(
                ui->plainTextEditInput->toPlainText()));
        }

    }
    else if (modeXOR) {
        auto* cryptoHelper = CryptoHelper::instance();
        if (!decrypt) {
            ui->plainTextEditOutput->setPlainText(cryptoHelper->encryptXOR(
                ui->plainTextEditInput->toPlainText(), ui->plainTextEditPassword->toPlainText()));
        }
        else {
            ui->plainTextEditOutput->setPlainText(cryptoHelper->decryptXOR(
                ui->plainTextEditInput->toPlainText(), ui->plainTextEditPassword->toPlainText()));
        }
            
    }
}

void EncriptorPage::onChangeDecrypt()
{
    if (ui->radioButtonShifrovanie->isChecked()) {
        decrypt = false;
    }
    else decrypt = true;
}
