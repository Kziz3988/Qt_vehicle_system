#ifndef MESSAGE_SERVER_H
#define MESSAGE_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDebug>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QByteArray>

class MessageServer : public QObject
{
    Q_OBJECT

public:
    MessageServer(QObject *parent = nullptr);
    //~MessageServer();

    void SendData(QString, int, QString, QTcpSocket*, QString = "", int = 0);
    void SendMessage(QString, QString);
    void CreateJsonFile(QString);

private:
    QString ShowIP(QTcpSocket*);
    void PrintLog(QString);
    void SaveData(QString, QVector<QString>*);
    QVector<QString>* LoadData(QString);

    QTcpServer* server = new QTcpServer();
    QVector<QTcpSocket*>* users = new QVector<QTcpSocket*>();
    QVector<QString>* userIPs = new QVector<QString>();
    QVector<QString>* userNames = new QVector<QString>();
    QVector<QString>* userPasswords = new QVector<QString>();
    QVector<QString>* userAvatars = new QVector<QString>();
    //QVector<QPair<QString, QString>*>* historicalMessages = new QVector<QPair<QString, QString>*>();
    QVector<QString>* hisMsgContent = new QVector<QString>();
    QVector<QString>* hisMsgSource = new QVector<QString>();
    QString log;
    int port;

private slots:
    void NewConnectionSlot();
    void DisconnectSlot();
    void ReadyReadSlot();
};

#endif // MESSAGE_SERVER_H
