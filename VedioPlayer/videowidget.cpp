#include "videowidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>

VideoWidget::VideoWidget(QWidget *parent)
    : QWidget(parent),
      videoList(new VideoList(this)),
      videoPlayer(new VideoPlayerWidget(this)) {

    //设置窗口大小800*600，不可调整（防止布局被打乱）
    setFixedSize(800, 600);
    setStyleSheet("background-color: #d5cabd;");

    //固定视频列表大小
    videoList->setFixedSize(200,500);

    //固定视频播放器大小
    videoPlayer->setFixedSize(500,500);

    //homeButton
    homeButton = new QPushButton(this);
    QIcon homeicon(":/img/home.png");
    homeButton->setIcon(homeicon);
    homeButton->setFixedSize(40, 40);

    //布局设置
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    QHBoxLayout *toplayout = new QHBoxLayout(this);
    QHBoxLayout *list_playerlayout = new QHBoxLayout(this);

    toplayout->addWidget(homeButton, 0, Qt::AlignLeft | Qt::AlignTop);
    mainlayout->addLayout(toplayout);
    mainlayout->addLayout(list_playerlayout);
    list_playerlayout->addWidget(videoList);  // 添加视频列表到布局中
    list_playerlayout->addWidget(videoPlayer);  // 添加视频播放器到布局中

    connect(videoList, &VideoList::videoSelected, this, &VideoWidget::onVideoSelected);
    setLayout(mainlayout);

    //链接home按钮的信号与槽函数
    connect(homeButton, &QPushButton::clicked, this, &VideoWidget::goHome);
}

void VideoWidget::goHome() {
    //在此实现返回主界面功能
    emit JumpToMainMenu();
    this->hide();
}

void VideoWidget::onVideoSelected(const QString &videoPath) {
    videoPlayer->playVideo(videoPath);  // 传递视频路径并播放
}

void VideoWidget::ShowVideoSlot()
{
    this->show();
}
