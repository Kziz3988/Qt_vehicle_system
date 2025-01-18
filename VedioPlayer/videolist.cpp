#include "videolist.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QIcon>

VideoList::VideoList(QWidget *parent)
    : QWidget(parent),
      listWidget(new QListWidget(this)) {

    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    QHBoxLayout *buttonlayout = new QHBoxLayout();

    QPushButton *addButton = new QPushButton(this);
    QPushButton *removeButton = new QPushButton(this);
    addButton->setIcon(QIcon(":/img/files.png"));
    removeButton->setIcon(QIcon(":/img/ashbin.png"));

    buttonlayout->addWidget(addButton);
    buttonlayout->addWidget(removeButton);
    mainlayout->addLayout(buttonlayout);
    mainlayout->addWidget(listWidget);

    connect(addButton, &QPushButton::clicked, this, &VideoList::addVideo);
    connect(removeButton, &QPushButton::clicked, this, &VideoList::removeVideo);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &VideoList::onItemDoubleClicked);

    setLayout(mainlayout);
}

void VideoList::addVideo() {
    QStringList files = QFileDialog::getOpenFileNames(this, "Add Videos", QString(), "Video Files (*.mp4 *.avi *.mkv)");
    foreach (const QString &file, files) {
        listWidget->addItem(file);
    }
}

void VideoList::removeVideo() {
    delete listWidget->takeItem(listWidget->currentRow());
}

void VideoList::onItemDoubleClicked(QListWidgetItem *item) {
    emit videoSelected(item->text());
}
