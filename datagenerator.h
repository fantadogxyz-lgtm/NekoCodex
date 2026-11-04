#pragma once

#include <QWidget>
#include "ui_datagenerator.h"
#include "datageneratorsettings.h"

class DataGenerator : public QWidget
{
	Q_OBJECT

public:
	DataGenerator(QWidget *parent = nullptr);
	~DataGenerator();

private:
	Ui::DataGenerator* ui;
	DataGeneratorSettings * dataGeneratorSettings;

private slots:
    void onDeleteClicked();
    void onGenerateDateClicked();
    void onGenerateEmailClicked();
    void onGenerateNameClicked();
    void onGenerateRandomWordClicked();
    void onGeneratePasswordNotEasyClicked();
    void onGeneratePhoneClicked();
    void onGenerateSurnameClicked();
    void onGenerateUsernameClicked();

    
private:
    QString getRandomWordFromFile(const QString& filePath);

};
