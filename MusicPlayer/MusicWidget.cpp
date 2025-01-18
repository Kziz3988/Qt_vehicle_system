//主窗口
#include "MusicWidget.h"
#include "ui_MusicWidget.h"
#include "QWidget"
#include "QVBoxLayout"

MusicWidget::MusicWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 600);
    setStyleSheet("background-color: #fefedf;");

    musicPlayerWidget = new MusicPlayerWidget(this);
    songListWidget = new MusicList(this);

    musicPlayerWidget->setFixedSize(500, 500);

    //back to menu
    homeButton = new QPushButton(this);
    QIcon homeIcon(":/musicimg/home.png");
    homeButton->setIcon(homeIcon);
    homeButton->setFixedSize(40, 40);

    //布局设置
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    QHBoxLayout *toplayout = new QHBoxLayout();
    QHBoxLayout *list_playerlayout = new QHBoxLayout();

    toplayout->addWidget(homeButton, 0, Qt::AlignLeft | Qt::AlignTop);
    mainlayout->addLayout(toplayout);
    mainlayout->addLayout(list_playerlayout);
    list_playerlayout->addWidget(songListWidget);
    list_playerlayout->addWidget(musicPlayerWidget);

    this->setLayout(mainlayout);

    songListWidget->setPlaylist(musicPlayerWidget->getplaylist());

    // 连接信号和槽
    connect(songListWidget, &MusicList::songAdded, [this](const QString &songName) {
        musicPlayerWidget->setSongInfo(songName); // 更新歌曲名
    });

    //Home键信号与槽函数
    connect(homeButton, &QPushButton::clicked, this, &MusicWidget::goHome);

}

void MusicWidget::goHome(){
    //在这里实现返回主界面函数定义
    emit JumpToMainMenu();
    this->hide();
}
void MusicWidget::ShowMusicSlot()
{
    this->show();
}

MusicWidget::~MusicWidget()
{

}

