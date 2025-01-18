#include "main_menu.h"
#include "ui_main_menu.h"

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    this->setWindowTitle("车载中控系统");
    this->setFixedSize(600,400);
    QPixmap background(":/new/R-C.jpg");
    QPalette pal;
    pal.setBrush(QPalette::Background, QBrush(background));
    this->setPalette(pal);
    //this->setStyleSheet("#main{background-image:url(:/new/R-C.jpg);}");
    SetButtonImage(":/new/image.png", ui->imageButton);
    SetButtonImage(":/new/music.png", ui->musicButton);
    SetButtonImage(":/new/video.png", ui->videoButton);
    SetButtonImage(":/new/weather.png", ui->weatherButton);
    SetButtonImage(":/new/explorer.png", ui->explorerButton);
    SetButtonImage(":/new/communication.png", ui->messageButton);

    connect(ui->weatherButton, SIGNAL(clicked(bool)), this, SLOT(JumpToWeatherSlot()));
    connect(ui->messageButton, SIGNAL(clicked(bool)), this, SLOT(JumpToMessageSlot()));
    connect(ui->imageButton, SIGNAL(clicked(bool)), this, SLOT(JumpToImageSlot()));
    connect(ui->musicButton, SIGNAL(clicked(bool)), this, SLOT(JumpToMusicSlot()));
    connect(ui->explorerButton, SIGNAL(clicked(bool)), this, SLOT(JumpToExplorerSlot()));
    connect(ui->videoButton, SIGNAL(clicked(bool)), this, SLOT(JumpToVideoSlot()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::SetButtonImage(QString path, QPushButton* button)
{
    button->setText("");
    button->setStyleSheet("QPushButton{ border:none; }");
    QPixmap* img = new QPixmap();
    img->load(path);
    button->setIcon(QIcon(*img));
    button->setIconSize(button->size());
}

void MainMenu::ShowMainMenuSlot()
{
    this->show();
}
void MainMenu::JumpToWeatherSlot()
{
    emit JumpToWeather();
    this->hide();
}
void MainMenu::JumpToMessageSlot()
{
    emit JumpToMessage();
    this->hide();
}
void MainMenu::JumpToExplorerSlot()
{
    emit JumpToExplorer();
    this->hide();
}
void MainMenu::JumpToImageSlot()
{
    emit JumpToImage();
    this->hide();
}
void MainMenu::JumpToMusicSlot()
{
    emit JumpToMusic();
    this->hide();
}
void MainMenu::JumpToVideoSlot()
{
    emit JumpToVideo();
    this->hide();
}
