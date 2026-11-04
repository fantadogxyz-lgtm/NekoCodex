#include "githelper.h"
#include "ui_githelper.h"

GitHelper::GitHelper(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::GitHelper())
{
	ui->setupUi(this);

	connect(ui->pushButtonStart, &QPushButton::clicked,
		this, &GitHelper::onStartClicked);

	connect(ui->pushButtonSave, &QPushButton::clicked,
		this, &GitHelper::onSaveClicked);

	connect(ui->pushButtonBrances, &QPushButton::clicked,
		this, &GitHelper::onBrancesClicked);

	connect(ui->pushButtonGitHub, &QPushButton::clicked,
		this, &GitHelper::onGitHubClicked);

	connect(ui->pushButtonCool, &QPushButton::clicked,
		this, &GitHelper::onCoolClicked);
}

GitHelper::~GitHelper()
{
	delete ui;
}

void GitHelper::onStartClicked() {
	ui->stackedWidget->setCurrentIndex(0);

	ui->pushButtonStart->setStyleSheet("background-color: rgb(162, 157, 194); color: rgb(226, 226, 226); ");

	ui->pushButtonSave->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonBrances->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonGitHub->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonCool->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
}


void GitHelper::onSaveClicked() {
	ui->stackedWidget->setCurrentIndex(1);

	ui->pushButtonSave->setStyleSheet("background-color: rgb(162, 157, 194); color: rgb(226, 226, 226); ");

	ui->pushButtonStart->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonBrances->setStyleSheet("background-color: rgb(85, 85, 127); color:rgb(226, 226, 226); ");
	ui->pushButtonGitHub->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonCool->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
}


void GitHelper::onBrancesClicked() {
	ui->stackedWidget->setCurrentIndex(2);

	ui->pushButtonBrances->setStyleSheet("background-color: rgb(162, 157, 194); color: rgb(226, 226, 226); ");

	ui->pushButtonStart->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonSave->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonGitHub->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonCool->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
}

void GitHelper::onGitHubClicked() {
	ui->stackedWidget->setCurrentIndex(3);

	ui->pushButtonGitHub->setStyleSheet("background-color: rgb(162, 157, 194); color: rgb(226, 226, 226); ");

	ui->pushButtonStart->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonSave->setStyleSheet("background-color: rgb(85, 85, 127); color:rgb(226, 226, 226); ");
	ui->pushButtonBrances->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonCool->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
}

void GitHelper::onCoolClicked() {
	ui->stackedWidget->setCurrentIndex(4);

	ui->pushButtonCool->setStyleSheet("background-color: rgb(162, 157, 194); color: rgb(226, 226, 226); ");

	ui->pushButtonStart->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonSave->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonBrances->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
	ui->pushButtonGitHub->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(226, 226, 226); ");
}
