#pragma once

#include <QWidget>

#include <QDialog>
#include "jsonfieldstructs.h"

#include <QUuid>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class FieldEditDialog; };
QT_END_NAMESPACE

class FieldEditDialog : public QWidget
{
	Q_OBJECT

public:
	FieldEditDialog(QWidget *parent = nullptr);
	~FieldEditDialog();

	void setFieldConfig(const FieldConfig& config);
	FieldConfig getFieldConfig() const;

signals:
	void saveRequested(const FieldConfig& config);
	void cancelRequested();                      

private slots:
	void onTypeChanged(int index);
	void onStringCategoryChanged(int index);
	void onSaveClicked();
	void onCancelClicked();


private:

	QString m_currentUuid;

	void initComboboxes();
	void updateUiForStringCategory(StringCategory category);

private:
	Ui::FieldEditDialog *ui;
};
