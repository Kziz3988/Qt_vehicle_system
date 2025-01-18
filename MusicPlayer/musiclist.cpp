#include "musiclist.h"
#include "QFileDialog"
#include "QFileInfo"
#include "QUrl"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QIcon"

MusicList::MusicList(QWidget *parent)
    : QWidget(parent){

    //创建按钮图标
    addBtn = new QPushButton(this);
    removeBtn = new QPushButton(this);
    QIcon addIcon(":/musicimg/files.png");
    QIcon removeIcon(":/musicimg/ashbin.png");

    addBtn->setIcon(addIcon);
    removeBtn->setIcon(removeIcon);

    addBtn->setFixedSize(40, 40);
    removeBtn->setFixedSize(40, 40);
    addBtn->setStyleSheet("QPushButton { border: none; background: transparent; }");
    removeBtn->setStyleSheet("QPushButton { border: none; background: transparent; }");


    musicListWidget = new QListWidget(this);

    //链接按钮的点击信号到槽函数
    connect(addBtn, &QPushButton::clicked, this, &MusicList::addSong);
    connect(removeBtn, &QPushButton::clicked, this, &MusicList::removeSong);

    //layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(removeBtn);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(buttonLayout);
    layout->addWidget(musicListWidget);
    this->setLayout(layout);
}

    void MusicList::setPlaylist(QMediaPlaylist *playlist) {
    this->playlist = playlist;
    }

    //定义添加歌曲函数
    void MusicList::addSong(){
        //选择音频文件(.mp3 .wav .flac .ogg)
        QStringList files = QFileDialog::getOpenFileNames(this,"Add Songs", QString(), "Audio Files (*.mp3 *.wav *.flac *.ogg)");
        if (!files.isEmpty()) {
            foreach(QString file, files) {
                //将所有音频文件添加到列表中
                playlist->addMedia(QUrl::fromLocalFile(file));
                QString fileName = QFileInfo(file).fileName();
                musicListWidget->addItem(fileName);
                emit songAdded(fileName); // 发射信号
            }
        }
    }

    void MusicList::removeSong(){
        int currentRow = musicListWidget->currentRow();
        if (currentRow >= 0){
            playlist->removeMedia(currentRow);
            delete musicListWidget->takeItem(currentRow);
        }
    }
