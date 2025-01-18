#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QMediaPlaylist>
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QFileDialog>

class MusicList : public QWidget
{
    Q_OBJECT

public:
    explicit MusicList(QWidget *parent = nullptr);

    //出现过的问题：起初将播放暂停、添加删除等功能集成在了MusicPlayerWidget中，
    //导致后续将添加删除功能移动至musiclist后无法播放歌曲，
    //认定原因为playlist在两个类之间没传递
    void setPlaylist(QMediaPlaylist *playlist);

private slots:
    void addSong();
    void removeSong();

signals:
    void songAdded(const QString &songname);

private:
    QPushButton *addBtn;
    QPushButton *removeBtn;

    //歌词窗口
    QListWidget *musicListWidget;

    QMediaPlaylist *playlist;
};

#endif // MUSICLIST_H
