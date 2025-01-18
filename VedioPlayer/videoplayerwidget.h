#ifndef VIDEOPLAYERWIDGET_H
#define VIDEOPLAYERWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>
#include <QPushButton>
#include <QIcon>

class VideoPlayerWidget : public QWidget {
    Q_OBJECT

public:
    explicit VideoPlayerWidget(QWidget *parent = nullptr);
    void playVideo(const QString &videoPath);
    void playPause();
    void stop();

private:
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QSlider *progressSlider;
    QSlider *volumeSlider;
    QPushButton *playButton;
    QPushButton *fullScreenButton;

    QIcon playIcon;
    QIcon pauseIcon;

private slots:
    void adjustVolume(int value);
    void adjustProgress(int value);
    void fastForward();
    void rewind();
    void toggleFullScreen();

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // VIDEOPLAYERWIDGET_H
