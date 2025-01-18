#ifndef VIDEOLIST_H
#define VIDEOLIST_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

class VideoList : public QWidget {
    Q_OBJECT

public:
    explicit VideoList(QWidget *parent = nullptr);

signals:
    void videoSelected(const QString &videoPath);

private slots:
    void addVideo();
    void removeVideo();
    void onItemDoubleClicked(QListWidgetItem *item);

private:
    QListWidget *listWidget;
};

#endif // VIDEOLIST_H
