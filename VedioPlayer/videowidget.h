#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include "videolist.h"
#include "videoplayerwidget.h"

class VideoWidget : public QWidget {
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = nullptr);

public slots:
    void ShowVideoSlot();
signals:
    void JumpToMainMenu();

private:
    QPushButton *homeButton;
    VideoList *videoList;
    VideoPlayerWidget *videoPlayer;

private slots:
    void onVideoSelected(const QString &videoPath);

    //Home按钮的槽函数
    void goHome();
};

#endif // VIDEOWIDGET_H
