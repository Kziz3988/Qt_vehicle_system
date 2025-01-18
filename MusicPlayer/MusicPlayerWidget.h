#ifndef MUSICPLAYERWIDGET_H
#define MUSICPLAYERWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>

class MusicPlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPlayerWidget(QWidget *parent = nullptr);
    QMediaPlaylist* getplaylist() const;

    //设置歌曲信息
    void setSongInfo(const QString &info);

private slots:
    void playPause();
    void nextTrack();
    void previousTrack();
    void changePlayMode();
    void adjustVolume(int value);
    void adjustProgress(int progress);

    //滚动文本
    //void scrollText();

private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;

    QPushButton *playBtn;
    QPushButton *modeBtn;
    QPushButton *nextBtn;
    QPushButton *previousBtn;
    QSlider *volumeSlider;
    QSlider *progressSlider;
    QLabel *coverLabel;//封面图片
    QLabel *infoLabel;//文本框
    QTimer *scrollTimer;//滚动速度
    QString fullInfoText;
    int scrollPosition;

    // 添加图标变量
    QIcon loopIcon;
    QIcon randomIcon;
    QIcon sequentialIcon;
    QIcon playIcon;
    QIcon pauseIcon;

    void updatePlayPauseBtn();
    void updateModeBtn();
    void updateProgressSlider();

};

#endif // MUSICPLAYERWIDGET_H
