#pragma once

#include <QWidget>
#include "cryptohelper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EncriptorPage; };
QT_END_NAMESPACE

class EncriptorPage : public QWidget
{
	Q_OBJECT

public:
	EncriptorPage(QWidget *parent = nullptr);
	~EncriptorPage();

private:
	Ui::EncriptorPage *ui;

	void hidePassword();
	void showPassword();

	bool modeAES256 = true;
	bool modeAES128= false;
	bool modeAESBase64 = false;
	bool modeXOR = false;

	bool decrypt = false;

private slots:
	void onButtonAES256();
	void onButtonAES128();
	void onButtonBase64();
	void onButtonXOR();

	void cleanTextOutputs();
	void makeOperation();

	void onChangeDecrypt();
};
