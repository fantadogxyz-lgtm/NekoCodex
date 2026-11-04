#pragma once

#include <QWidget>

#include <QFileDialog>
#include <QString>
#include <QDesktopServices>  
#include <QUrl>              
#include <QMessageBox>       
#include <QProcess>
#include <QTimer>
#include <QLineEdit>
#include "Neko_settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QuickWidgSettings; };
QT_END_NAMESPACE

class QuickWidgSettings : public QWidget
{
	Q_OBJECT

public:
	QuickWidgSettings(QWidget *parent = nullptr);
	~QuickWidgSettings();

private:
	Ui::QuickWidgSettings *ui;

	QString m_filePath_1;
	QString m_filePath_2;
	QString m_filePath_3;
	QString m_filePath_4;
	QString m_filePath_5;
	QString m_filePath_6;
	QString m_filePath_7;
	QString m_filePath_8;
	QString m_filePath_9;

	QTimer* m_autoSaveTimer = nullptr;
	QString m_filePaths[9];
	void autoSaveData(int index, const QString& text);
	void cleanSlot(int index);

	QLineEdit* getLineEditButtonText(int index);

	void loadSettingsToUI();

private slots:
	void on_pushButtonBrowse_1_clicked();
	void on_pushButtonBrowse_2_clicked();
	void on_pushButtonBrowse_3_clicked();
	void on_pushButtonBrowse_4_clicked();
	void on_pushButtonBrowse_5_clicked();
	void on_pushButtonBrowse_6_clicked();
	void on_pushButtonBrowse_7_clicked();
	void on_pushButtonBrowse_8_clicked();
	void on_pushButtonBrowse_9_clicked();

	void on_pushButtonClean_1_clicked();
	void on_pushButtonClean_2_clicked();
	void on_pushButtonClean_3_clicked();
	void on_pushButtonClean_4_clicked();
	void on_pushButtonClean_5_clicked();
	void on_pushButtonClean_6_clicked();
	void on_pushButtonClean_7_clicked();
	void on_pushButtonClean_8_clicked();
	void on_pushButtonClean_9_clicked();


	void onAnyLineEditTextChanged(const QString& text);
	void triggerAutoSave();



};
