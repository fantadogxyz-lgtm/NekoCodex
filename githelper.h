#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class GitHelper; };
QT_END_NAMESPACE

class GitHelper : public QWidget
{
	Q_OBJECT

public:
	GitHelper(QWidget *parent = nullptr);
	~GitHelper();

private:
	Ui::GitHelper *ui;

private slots:
	void onStartClicked();
	void onSaveClicked();
	void onBrancesClicked();
	void onGitHubClicked();
	void onCoolClicked();
};
