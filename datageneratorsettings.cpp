#include "datageneratorsettings.h"

DataGeneratorSettings::DataGeneratorSettings(QWidget *parent)
	: QWidget(parent), ui(new Ui::DataGeneratorSettings)
{
    qDebug() << "Address of ui pointer:" << &ui;


    qDebug() << "Value of ui (points to):" << ui;


    qDebug() << "Size of Ui::DataGenerator:" << sizeof(Ui::DataGeneratorSettings);

    ui->setupUi(this);
    qDebug() << "ui->setupUi completed";
    qDebug() << "After setupUi - ui value:" << ui;

    setStyleSheet(R"(
        QCheckBox {
            border: none;
            background-color: transparent;
            
        }
        QCheckBox::indicator {
            width: 10px;
            height: 10px;
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
            width: 14px;  /* чуть больше для радио, т.к. круглому нужно больше места */
            height: 14px;
    
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

    ui->dateEditDateMin->setDate(QDate(2000, 1, 1)); 
    ui->dateEditDateMax->setDate(QDate::currentDate()); 
    ui->radioButtonDateFormat2->setChecked(true);
}

DataGeneratorSettings::~DataGeneratorSettings()
{
}

bool DataGeneratorSettings::isNameEnglish() const {
    return ui->radioButtonNameOnEnglish->isChecked();
}

int DataGeneratorSettings::getNameMinLength() const {
    return ui->spinBoxNameMin->value();
}

int DataGeneratorSettings::getNameMaxLength() const {
    return ui->spinBoxNameMax->value();
}

bool DataGeneratorSettings::isSurnameEnglish() const {
    return ui->radioButtonSurnameOnEnglish->isChecked();
}

int DataGeneratorSettings::getSurnameMinLength() const {
    return ui->spinBoxSurnameMin->value();
}

int DataGeneratorSettings::getSurnameMaxLength() const {
    return ui->spinBoxSurnameMax->value();
}

bool DataGeneratorSettings::isUsernameSymbols() const {
    return ui->checkBoxUsernameSpecificSimbols->isChecked(); 
}

bool DataGeneratorSettings::isUsernameNumbers() const
{
    return ui->checkBoxUsernameNumbs->isChecked();
}

int DataGeneratorSettings::getUsernameMinLength() const
{
    return ui->spinBoxUsernameMin->value();
}

int DataGeneratorSettings::getUsesnameMaxLength() const
{
    return ui->spinBoxUsernameMax->value();
}

int DataGeneratorSettings::getEmailMinLength() const {

    return ui->spinBoxEmailMin->value();
}

int DataGeneratorSettings::getEmailMaxLength() const {
    return ui->spinBoxEmailMax->value();
}

bool DataGeneratorSettings::isEmailGmail() const {
    return ui->radioButtonEmailIsGmail->isChecked();
}

bool DataGeneratorSettings::isEmailYandex() const {
    return ui->radioButtonEmailIsYandex->isChecked();
}

bool DataGeneratorSettings::isEmailMailru() const {
    return ui->radioButtonEmailIsMailru->isChecked();
}

bool DataGeneratorSettings::isPhoneFormat1() const {
    return ui->radioButtonPhoneFormat1->isChecked(); // 37529xxxxxxx
}

bool DataGeneratorSettings::isPhoneFormat2() const {
    return ui->radioButtonPhoneFormat2->isChecked(); // +375 (29) xxxxxxx
}

bool DataGeneratorSettings::isPhoneFormat3() const {
    return ui->radioButtonPhoneFormat3->isChecked(); // 8029xxxxxxx
}

bool DataGeneratorSettings::isPhoneFormat4() const {
    return ui->radioButtonPhoneFormat4->isChecked(); // 8 (029) xxxxxxx
}

int DataGeneratorSettings::getPasswordMinLength() const {
    return ui->spinBoxPasswordMin->value();
}

int DataGeneratorSettings::getPasswordMaxLength() const {
    return ui->spinBoxPasswordMax->value();
}

bool DataGeneratorSettings::isPasswordSpecificSymbols() const {
    return ui->checkBoxPasswordSpecificSimbols->isChecked();
}

bool DataGeneratorSettings::isPasswordOnlyLetters() const {
    return ui->radioButtonPasswordIsOnlyLetters->isChecked();
}

bool DataGeneratorSettings::isPasswordOnlyNumbers() const {
    return ui->radioButtonPasswordIsOnlyNumbers->isChecked();
}

bool DataGeneratorSettings::isPasswordLettersAndNumbers() const {
    return ui->radioButtonPasswordIsLettersAndNumbers->isChecked();
}

int DataGeneratorSettings::getRandomWordMinLength() const
{
    return ui->spinBoxWordMin->value();
}

int DataGeneratorSettings::getRandomWordMaxLength() const
{
    return ui->spinBoxWordMax->value();
}

bool DataGeneratorSettings::isWordEnglish() const {
    return ui->radioButtonWordOnEnglish->isChecked();
}

bool DataGeneratorSettings::isWordRus() const {
    return ui->radioButtonWordOnRus->isChecked();
}

QDate DataGeneratorSettings::getDateMin() const {
    return ui->dateEditDateMin->date();
}

QDate DataGeneratorSettings::getDateMax() const {
    return ui->dateEditDateMax->date();
}

bool DataGeneratorSettings::isDateFormat1() const {
    return ui->radioButtonDateFormat1->isChecked(); // YYYY-MM-DD
}

bool DataGeneratorSettings::isDateFormat2() const {
    return ui->radioButtonDateFormat2->isChecked(); // DD.MM.YYYY
}

bool DataGeneratorSettings::isDateFormat3() const {
    return ui->radioButtonDateFormat3->isChecked(); // DD/MM/YYYY
}
