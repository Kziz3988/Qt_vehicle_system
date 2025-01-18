#include "MusicPlayerWidget.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>

MusicPlayerWidget::MusicPlayerWidget(QWidget *parent)
    :QWidget(parent),
      player(new QMediaPlayer(this)),
      playlist(new QMediaPlaylist(this)),
      scrollPosition(0),
      loopIcon(":/musicimg/singlecycle.png"),  // 设置图标路径
      randomIcon(":/musicimg/randomplay.png"),  // 设置图标路径
      sequentialIcon(":/musicimg/repeat.png"),  // 设置图标路径
      playIcon(":/musicimg/start.png"),
      pauseIcon(":/musicimg/pause.png")
{
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    player->setPlaylist(playlist);

    QIcon next(":/musicimg/next.png");
    QIcon previous(":/musicimg/last.png");


        // 初始化UI组件
        previousBtn = new QPushButton(this);
        playBtn = new QPushButton(this);
        nextBtn = new QPushButton(this);
        modeBtn = new QPushButton(this);
        volumeSlider = new QSlider(Qt::Horizontal, this);
        progressSlider = new QSlider(Qt::Horizontal, this);

        //添加图标
        playBtn->setIcon(playIcon);
        playBtn->setFixedSize(40, 40);  // 设置按钮大小
        playBtn->setIconSize(QSize(40, 40));  // 设置图标大小
        playBtn->setStyleSheet("QPushButton { border: none; background: transparent; }");

        nextBtn->setIcon(next);
        nextBtn->setFixedSize(40, 40);  // 设置按钮大小
        nextBtn->setIconSize(QSize(40, 40));  // 设置图标大小
        nextBtn->setStyleSheet("QPushButton { border: none; background: transparent; }");

        previousBtn->setIcon(previous);
        previousBtn->setFixedSize(40, 40);  // 设置按钮大小
        previousBtn->setIconSize(QSize(40, 40));  // 设置图标大小
        previousBtn->setStyleSheet("QPushButton { border: none; background: transparent; }");

        modeBtn->setIcon(sequentialIcon);
        modeBtn->setFixedSize(40, 40);
        modeBtn->setIconSize(QSize(40, 40));
        modeBtn->setStyleSheet("QPushButton { border: none; background: transparent; }");

        // 设置固定的封面图片
        coverLabel = new QLabel(this);
        QPixmap coverPixmap(":/musicimg/cover.png");  // 替换为实际图片路径
        coverLabel->setPixmap(coverPixmap.scaled(300, 300, Qt::KeepAspectRatio));
        coverLabel->setFixedSize(300, 300);

        // 设置滚动文本标签
        infoLabel = new QLabel("当前暂无歌曲", this);
        infoLabel->setFixedSize(300, 30);  // 设置文本区域大小
        infoLabel->setStyleSheet("backgroundbackground: transparent; color: black;");  // 设置背景色和文本颜色


        // 设置UI布局
        QHBoxLayout *buttonlayout = new QHBoxLayout;
        QVBoxLayout *sliderlayout = new QVBoxLayout;
        buttonlayout->addWidget(previousBtn);
        buttonlayout->addWidget(playBtn);
        buttonlayout->addWidget(nextBtn);
        buttonlayout->addWidget(modeBtn);
        buttonlayout->addWidget(volumeSlider);
        sliderlayout->addWidget(coverLabel, 0,Qt::AlignCenter);
        sliderlayout->addWidget(infoLabel, 0,Qt::AlignCenter);
        sliderlayout->addWidget(progressSlider);
        sliderlayout->addLayout(buttonlayout);

        this->setLayout(sliderlayout);

        // 定时器用于滚动文本
//        scrollTimer = new QTimer(this);
//        connect(scrollTimer, &QTimer::timeout, this, &MusicPlayerWidget::scrollText);

        // 设置滚动速度，100ms为滚动间隔
//        scrollTimer->start(100);

        // 连接信号与槽
        connect(playBtn, &QPushButton::clicked, this, &MusicPlayerWidget::playPause);
        connect(nextBtn, &QPushButton::clicked, this, &MusicPlayerWidget::nextTrack);
        connect(previousBtn, &QPushButton::clicked, this, &MusicPlayerWidget::previousTrack);
        connect(modeBtn, &QPushButton::clicked, this, &MusicPlayerWidget::changePlayMode);
        connect(volumeSlider, &QSlider::valueChanged, this, &MusicPlayerWidget::adjustVolume);
        connect(progressSlider, &QSlider::valueChanged, this, &MusicPlayerWidget::adjustProgress);

        // 初始化播放器设置
        volumeSlider->setRange(0, 100);

        //设置初始音量为50
        volumeSlider->setValue(50);
        player->setVolume(50);
        progressSlider->setRange(0, player->duration());

        // 连接进度条和播放器
        connect(player, &QMediaPlayer::stateChanged, this, &MusicPlayerWidget::updatePlayPauseBtn);
        connect(player, &QMediaPlayer::positionChanged, this, &MusicPlayerWidget::updateProgressSlider);
    }

    //实现函数定义
    void MusicPlayerWidget::setSongInfo(const QString &info) {
        if (info.isEmpty()) {
            infoLabel->setText("当前暂无歌曲");  // 显示默认文本
        } else {
            infoLabel->setText(info);  // 显示歌曲名
        }
    }

//    void MusicPlayerWidget::scrollText() {
//        scrollPosition++;
//        if (scrollPosition > fullInfoText.length()) {
//            scrollPosition = 0;
//        }
//        QString textToShow = fullInfoText.mid(scrollPosition) + " " + fullInfoText.left(scrollPosition);
//        infoLabel->setText(textToShow.left(30));  // 显示30个字符
//    }

    //播放暂停
    void MusicPlayerWidget::playPause() {
        if (player->state() == QMediaPlayer::PlayingState) {
            player->pause();
        } else {
            player->play();
        }
        updatePlayPauseBtn();
    }

    //下一首歌
    void MusicPlayerWidget::nextTrack(){
        playlist->next();
        player->play();
    }

    //上一首歌
    void MusicPlayerWidget::previousTrack(){
        playlist->previous();
        player->play();
    }

    //调整音量
    void MusicPlayerWidget::adjustVolume(int value) {
        player->setVolume(value);
    }

    //调整进度条
    void MusicPlayerWidget::adjustProgress(int value) {
        player->setPosition(value);
    }

    //更新播放按钮
    void MusicPlayerWidget::updatePlayPauseBtn() {
        if (player->state() == QMediaPlayer::PlayingState) {
            playBtn->setIcon(pauseIcon);
        } else {
            playBtn->setIcon(playIcon);
        }
    }

    //更新模式按钮
    void MusicPlayerWidget::changePlayMode() {
        QMediaPlaylist::PlaybackMode mode = playlist->playbackMode();
        if (mode == QMediaPlaylist::Sequential) {
            playlist->setPlaybackMode(QMediaPlaylist::Loop);
            modeBtn->setIcon(loopIcon);
        } else if (mode == QMediaPlaylist::Loop) {
            playlist->setPlaybackMode(QMediaPlaylist::Random);
            modeBtn->setIcon(randomIcon);
        } else {
            playlist->setPlaybackMode(QMediaPlaylist::Sequential);
            modeBtn->setIcon(sequentialIcon);
        }
    }

    void MusicPlayerWidget::updateProgressSlider(){
        progressSlider->setRange(0, player->duration());
        progressSlider->setValue(player->position());
    }

    QMediaPlaylist* MusicPlayerWidget::getplaylist() const{
        return playlist;
    }
