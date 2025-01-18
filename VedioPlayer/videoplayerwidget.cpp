#include "videoplayerwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QIcon>

VideoPlayerWidget::VideoPlayerWidget(QWidget *parent)
    : QWidget(parent),
      player(new QMediaPlayer(this)),
      videoWidget(new QVideoWidget(this)),
      progressSlider(new QSlider(Qt::Horizontal, this)),
      volumeSlider(new QSlider(Qt::Horizontal, this)),
      playButton(new QPushButton(this)),
      fullScreenButton(new QPushButton(this)) {

    //设置图标
    playIcon = QIcon(":/img/start.png");
    pauseIcon = QIcon(":/img/pause.png");
    QIcon kuaijin(":/img/kuaijin.png");
    QIcon kuaitui(":/img/kuaitui.png");
    QIcon full(":/img/full.png");
    QIcon volume(":/img/volume.png");

    QPushButton *fastForwardButton = new QPushButton(this);
    QPushButton *rewindButton = new QPushButton(this);

    // 可选样式优化
    playButton->setIcon(playIcon);
    playButton->setFixedSize(40, 40);  // 设置按钮大小
    playButton->setIconSize(QSize(40, 40));  // 设置图标大小
    playButton->setStyleSheet("QPushButton { border: none; background: transparent; }");

    fastForwardButton->setIcon(kuaijin);
    fastForwardButton->setFixedSize(40, 40);  // 设置按钮大小
    fastForwardButton->setIconSize(QSize(40, 40));  // 设置图标大小
    fastForwardButton->setStyleSheet("QPushButton { border: none; background: transparent; }");

    rewindButton->setIcon(kuaitui);
    rewindButton->setFixedSize(40, 40);  // 设置按钮大小
    rewindButton->setIconSize(QSize(40, 40));  // 设置图标大小
    rewindButton->setStyleSheet("QPushButton { border: none; background: transparent; }");

    fullScreenButton->setIcon(full);
    fullScreenButton->setFixedSize(40, 40);  // 设置按钮大小
    fullScreenButton->setIconSize(QSize(40, 40));  // 设置图标大小
    fullScreenButton->setStyleSheet("QPushButton { border: none; background: transparent; }");

    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *controlLayout = new QHBoxLayout;

    controlLayout->addWidget(rewindButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(fastForwardButton);
    controlLayout->addWidget(volumeSlider);
    controlLayout->addWidget(fullScreenButton);

    layout->addWidget(videoWidget);
    layout->addWidget(progressSlider);
    layout->addLayout(controlLayout);

    player->setVideoOutput(videoWidget);

    videoWidget->setFocusPolicy(Qt::StrongFocus);

    connect(playButton, &QPushButton::clicked, this, &VideoPlayerWidget::playPause);

    // 连接播放器状态变化信号以自动更新按钮图标
    connect(player, &QMediaPlayer::stateChanged, this, [&](QMediaPlayer::State state) {
        if (state == QMediaPlayer::PlayingState) {
            playButton->setIcon(pauseIcon);
        } else {
            playButton->setIcon(playIcon);
        }
    });

    connect(fastForwardButton, &QPushButton::clicked, this, &VideoPlayerWidget::fastForward);
    connect(rewindButton, &QPushButton::clicked, this, &VideoPlayerWidget::rewind);
    connect(fullScreenButton, &QPushButton::clicked, this, &VideoPlayerWidget::toggleFullScreen);
    connect(volumeSlider, &QSlider::valueChanged, this, &VideoPlayerWidget::adjustVolume);
    connect(progressSlider, &QSlider::valueChanged, this, &VideoPlayerWidget::adjustProgress);

    setLayout(layout);
}

void VideoPlayerWidget::playVideo(const QString &videoPath) {
    player->setMedia(QUrl::fromLocalFile(videoPath));
    player->play();

    //链接媒体播放器的durationChanged 信号，设置进度条的最大值为视频总时长
    connect(player, &QMediaPlayer::durationChanged, this, [this](qint64 duration){
        progressSlider->setMaximum(static_cast<int>(duration));
    });

    //链接媒体播放器的positionChanged信号，更新进度条位置
    connect(player, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        progressSlider->setValue(static_cast<int>(position));
    });
}

void VideoPlayerWidget::playPause() {
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
    } else {
        player->play();
    }
}

void VideoPlayerWidget::stop() {
    player->stop();
}

void VideoPlayerWidget::adjustVolume(int value) {
    player->setVolume(value);
}

void VideoPlayerWidget::adjustProgress(int value) {
    player->setPosition(static_cast<qint64>(value));
}

void VideoPlayerWidget::fastForward() {
    player->setPosition(player->position() + 10000); // 快进10秒
}

void VideoPlayerWidget::rewind() {
    player->setPosition(player->position() - 10000); // 快退10秒
}

void VideoPlayerWidget::toggleFullScreen() {
    if (videoWidget->isFullScreen()) {
        videoWidget->setFullScreen(false);
        this->setFocus();  // 退出全屏后将焦点返回到主窗口
    } else {
        videoWidget->setFullScreen(true);
        videoWidget->setFocus();  // 进入全屏时确保 videoWidget 获得焦点
    }
}

void VideoPlayerWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        if (videoWidget->isFullScreen()) {
            videoWidget->setFullScreen(false);
            this->setFocus();  // 退出全屏后将焦点返回到主窗口
            return;
        }
    }
    QWidget::keyPressEvent(event);  // 处理其他键盘事件
}
