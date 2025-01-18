//主窗口
#ifndef MUSICWIDGET_H
#define MUSICWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "MusicPlayerWidget.h"
#include "musiclist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MusicWidget;
}
QT_END_NAMESPACE

class MusicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MusicWidget(QWidget *parent = nullptr);
    ~MusicWidget();

public slots:
    void ShowMusicSlot();
signals:
    void JumpToMainMenu();

private:
    MusicPlayerWidget *musicPlayerWidget;
    MusicList *songListWidget;

    //back-to-mainwindow button
    QPushButton *homeButton;

private slots:
    void goHome();
};
#endif // MUSICWIDGET_H
