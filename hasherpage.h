#pragma once

#include <QWidget>
#include <QCryptographicHash>
QT_BEGIN_NAMESPACE
namespace Ui { class HasherPage; };
QT_END_NAMESPACE

class HasherPage : public QWidget
{
	Q_OBJECT

public:
	HasherPage(QWidget *parent = nullptr);
	~HasherPage();

private:
	Ui::HasherPage *ui;

private slots:
	void onMakeOperationPressed();
	void onCleanPressed();

};
