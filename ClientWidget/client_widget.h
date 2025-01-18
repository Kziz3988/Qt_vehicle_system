#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTcpSocket>
#include <QHostAddress>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLayoutItem>
#include <QVector>
#include <QLayout>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QFileDialog>
#include <QByteArray>
#include <QBuffer>
#include <QStandardPaths>
#include <QVector>
#include <QFont>
#include <QTextCharFormat>
#include <QTextCursor>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWidget; }
QT_END_NAMESPACE

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();
    int maxCharInOneRow = 15;
    /*int widthOffset=8;
    int widthPreset=50;
    int heightOffset=18;
    int heightPreset=27;*/
    int widthOffset=9;
    int widthPreset=50;
    int heightOffset=18;
    int heightOffsetFullWidth=24;
    int heightPreset=30;
    int msgMargin = 5;
    static const QString emojis[25];

    QPixmap LoadPngImage(QString);

public slots:
    void ShowClientSlot();
signals:
    void JumpToMainMenu();

private:
    Ui::ClientWidget *ui;
    QString name;
    QTcpSocket* socket;
    int connectState;
    QPixmap defaultAvatar, currentAvatar, selectedAvatar;
    QVector<QString>* messageSource = new QVector<QString>();
    QVector<QLabel*>* messageAvatar = new QVector<QLabel*>();
    static const QFont emojiFont;

    void SendData(QString, int, QString rm = "");
    void NewMessage(QString, QString, QString);
    void HideEmoji();
    void SendImage(QPixmap, int);
    void SetEmojiFont(QTextEdit*);
    void mousePressEvent(QMouseEvent *event) override {
        ClientWidget::HideEmoji();
        //QWidget::mousePressEvent(event);
    }

private slots:
    void ChangeNameSlot();
    void ConnectSlot();
    void ConnectedSlot();
    void DisconnectSlot();
    void DisconnectedSlot();
    void ReadyReadSlot();
    void SendMessageSlot();
    void ShowEmojiSlot();
    void InsertEmojiSlot();
    void SelectAvatarSlot();
    void MessageChangedSlot();
    void JumpToMainMenuSlot();
};
#endif // CLIENTWIDGET_H
