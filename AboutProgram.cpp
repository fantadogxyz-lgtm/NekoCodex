#include "AboutProgram.h"
#include "ui_AboutProgram.h"

AboutProgram::AboutProgram(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::AboutProgram())
{
	ui->setupUi(this);

    if (!aboutProgramInWindow) {
        aboutProgramInWindow = new AboutProgramInWindow(this);
    }

    ui->scrollArea->setWidget(aboutProgramInWindow);
    ui->scrollArea->setWidgetResizable(true);

    aboutProgramInWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    aboutProgramInWindow->setMinimumHeight(2790);

    auto* randPic1 = new RandomPic();
    randPic1->setAttribute(Qt::WA_DeleteOnClose);
    randPic1->setAttribute(Qt::WA_QuitOnClose, false);
    //randPic1->setWindowFlags(randPic1->windowFlags() | Qt::WindowStaysOnTopHint);
    randPic1->setFixedSize(randPic1->size());
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();
    randPic1->move(screenRect.left(), screenRect.center().y() - (randPic1->height() / 2));
    randPic1->show();
}

AboutProgram::~AboutProgram()
{
	delete ui;



}
