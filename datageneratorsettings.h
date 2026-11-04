#pragma once

#include <QWidget>
#include "ui_datageneratorsettings.h"
#include <QString>
#include <QFile>
#include <QRandomGenerator>

class DataGeneratorSettings : public QWidget
{
	Q_OBJECT

public:
	DataGeneratorSettings(QWidget *parent = nullptr);
	~DataGeneratorSettings();

private:
	Ui::DataGeneratorSettings* ui;


public:
    bool isNameEnglish() const;
    int getNameMinLength() const;
    int getNameMaxLength() const; 
    
    bool isSurnameEnglish() const;
    int getSurnameMinLength() const;
    int getSurnameMaxLength() const;

    bool isUsernameSymbols() const;
    bool isUsernameNumbers() const;
    int getUsernameMinLength() const;
    int getUsesnameMaxLength() const;

    int getEmailMinLength() const;

    int getEmailMaxLength() const;

    bool isEmailGmail() const;

    bool isEmailYandex() const;

    bool isEmailMailru() const;

    bool isPhoneFormat1() const;

    bool isPhoneFormat2() const;

    bool isPhoneFormat3() const;

    bool isPhoneFormat4() const;

    int getPasswordMinLength() const;

    int getPasswordMaxLength() const;

    bool isPasswordSpecificSymbols() const;

    bool isPasswordOnlyLetters() const;

    bool isPasswordOnlyNumbers() const;

    bool isPasswordLettersAndNumbers() const;


    int getRandomWordMinLength() const;
    int getRandomWordMaxLength() const;
    bool isWordEnglish() const;
    bool isWordRus() const;
    QDate getDateMin() const;
    QDate getDateMax() const;
    bool isDateFormat1() const;
    bool isDateFormat2() const;
    bool isDateFormat3() const;
};
