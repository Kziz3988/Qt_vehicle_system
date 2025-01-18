#include "message_server.h"

MessageServer::MessageServer(QObject *parent)
{
    CreateJsonFile("userIPs.json");
    CreateJsonFile("userNames.json");
    CreateJsonFile("userPasswords.json");
    CreateJsonFile("userAvatars.json");
    CreateJsonFile("hisMsgContent.json");
    CreateJsonFile("hisMsgSource.json");
    userIPs = LoadData("userIPs.json");
    userNames = LoadData("userNames.json");
    userPasswords = LoadData("userPasswords.json");
    userAvatars = LoadData("userAvatars.json");
    hisMsgContent = LoadData("hisMsgContent.json");
    hisMsgSource = LoadData("hisMsgSource.json");
    port = 8000;
    connect(server, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
    if (server->listen(QHostAddress::Any, port))
    {
        PrintLog("Server started on port "+QString::number(port));
    }
    else
    {
        PrintLog("Server failed to start. Error: "+server->errorString());
    }
}

//Send data to client
void MessageServer::SendData(QString msg, int type, QString rm, QTcpSocket* socket, QString a, int e)
{
    QJsonObject msgObj;
    msgObj["message"] = msg.toUtf8().toStdString().data();
    msgObj["type"] = type;
    msgObj["remark"] = rm;
    msgObj["avatar"] = a;
    msgObj["extra"] = e;
    QJsonDocument jsonDoc(msgObj);
    socket->write(jsonDoc.toJson());
}

//Send public message to all users online
void MessageServer::SendMessage(QString msg, QString username)
{
    int count=0;
    for(int i=0; i<users->length(); i++)
    {
        if (users->at(i)->isOpen() && users->at(i)->isWritable() && (users->at(i))->state())
        {
            //This user is ready to receive message
            //users->at(i)->write(msg.toUtf8());
            QString avatar = userAvatars->at(userNames->indexOf(username));
            SendData(msg.toUtf8(), 0, username, users->at(i), avatar);
            count++;
        }
    }
    PrintLog("Successfully send message: '"+msg+"' to "+QString::number(count)+" users");
}

//Show IP of online user
QString MessageServer::ShowIP(QTcpSocket* user)
{
    return (user->peerAddress()).toString()+":"+QString::number(user->peerPort());
}
//Print log to server terminal
void MessageServer::PrintLog(QString log)
{
    qDebug("%s", (QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+": "+log).toStdString().data());
}

//A client has connected to server
void MessageServer::NewConnectionSlot()
{
    QTcpSocket* user = server->nextPendingConnection();
    users->push_back(user);
    //userIPs->push_back(ShowIP(user));
    //userNames->push_back("");
    //userPasswords->push_back("123456");
    connect(user, SIGNAL(disconnected()), this, SLOT(DisconnectSlot()));
    connect(user, SIGNAL(readyRead()), this, SLOT(ReadyReadSlot()));
    PrintLog(ShowIP(user)+" has connected to server");
}
//A client has disconnected from server
void MessageServer::DisconnectSlot()
{
    QTcpSocket* user = (QTcpSocket*)QObject::sender();
    //Remove user from list
    for(int i=0; i<users->length(); i++)
    {
        if(userIPs->at(i)==ShowIP(user))
        {
            PrintLog(userIPs->at(i)+" ("+userNames->at(i)+") has disconnected from server");
            break;
        }
    }
}
//Received a message from a client
void MessageServer::ReadyReadSlot()
{
    QTcpSocket* user = (QTcpSocket*)QObject::sender();
    QJsonObject msgObj = (QJsonDocument::fromJson(user->readAll())).object();
    QString message = msgObj["message"].toString();
    QString remark = msgObj["remark"].toString();
    //PrintLog(QString::number(msgObj["type"].toInt()));
    //QJsonDocument jsonDoc(msgObj);
    //PrintLog(jsonDoc.toJson().toStdString().data());
    switch(msgObj["type"].toInt())
    {
        case -1://User login/register
            if(userNames->contains(message))
            {
                int uid = userNames->indexOf(message);
                //PrintLog(QString::number(uid));
                if(userPasswords->at(uid) == remark)
                {
                    //Login successfully
                    userIPs->replace(uid, ShowIP(user));
                    SaveData("userIPs.json", userIPs);
                    PrintLog(ShowIP(user)+" ("+message+") logged in");
                    SendData(message, -1, userAvatars->at(uid), user);
                }
                else
                {
                    //Password is incorrect
                    PrintLog(ShowIP(user)+" entered a wrong password");
                    //user->close();
                }
            }
            else
            {
                //Register a new user
                userIPs->push_back(ShowIP(user));
                userNames->push_back(message);
                userPasswords->push_back(remark);
                userAvatars->push_back("");
                SaveData("userIPs.json", userIPs);
                SaveData("userNames.json", userNames);
                SaveData("userPasswords.json", userPasswords);
                SaveData("userAvatars.json", userAvatars);
                SendData(message, -2, "", user);
                PrintLog(ShowIP(user)+" ("+message+") registered");
            }
            break;
        case 0://User sent a message
            //historicalMessages->append(new QPair<QString, QString>(message, remark));
            hisMsgContent->append(message);
            hisMsgSource->append(remark);
            SaveData("hisMsgContent.json", hisMsgContent);
            SaveData("hisMsgSource.json", hisMsgSource);
            SendMessage(message, remark);
            PrintLog(ShowIP(user)+" ("+remark+") sent a message: "+message);
            break;
        case 1://User changes name
            if(userNames->count(message) == 0)//No duplicate name
            {
                /*for(int i=0; i<historicalMessages->size(); i++)
                {
                    if(historicalMessages->at(i)->second == remark)
                    {
                        historicalMessages->at(i)->second = message;
                    }
                }*/
                for(int i=0; i<hisMsgContent->size(); i++)
                {
                    if(hisMsgSource->at(i) == remark)
                    {
                        hisMsgSource->replace(i, message);
                    }
                }
                SaveData("hisMsgSource.json", hisMsgSource);
                for(int i=0; i<users->length(); i++)
                {
                    if (users->at(i)->isOpen() && users->at(i)->isWritable() && (users->at(i))->state())
                    {
                        //This user is ready to receive message
                        //users->at(i)->write(msg.toUtf8());
                        SendData(message, 4, remark, users->at(i));
                    }
                }
                PrintLog(ShowIP(user)+" ("+remark+") changed name to: "+message);
                userNames->replace(users->indexOf(user), message);
                SaveData("userNames.json", userNames);
            }
            else
            {
                SendData("", 5, "", user);
                PrintLog(ShowIP(user)+" ("+remark+") tried to change name to a invalid name: "+message);
            }
            break;
        case 2://Client confirms receipt
            PrintLog(ShowIP(user)+" ("+userNames->at(userIPs->indexOf(ShowIP(user)))+") received message: "+message);
            break;
        case 3://Change avatar
            //PrintLog(ShowIP(user));
            userAvatars->replace(userNames->indexOf(remark), message);
            SaveData("userAvatars.json", userAvatars);
            PrintLog(ShowIP(user)+" ("+remark+") changed avatar");
            for(int i=0; i<users->length(); i++)
            {
                if (users->at(i)->isOpen() && users->at(i)->isWritable() && (users->at(i))->state())
                {
                    //This user is ready to receive message
                    //users->at(i)->write(msg.toUtf8());
                    SendData(message, 1, remark, users->at(i));
                }
            }
            break;
        case 4://Upload avatar of new user
            userAvatars->replace(userNames->indexOf(remark), message);
            SaveData("userAvatars.json", userAvatars);
            //PrintLog("size:"+QString::number(historicalMessages->size())+",length:"+QString::number(historicalMessages->length())+",count:"+QString::number(historicalMessages->count()));
            /*if(historicalMessages->size()>0){
                //SendMessage(historicalMessages->at(0)->first, historicalMessages->at(0)->second);
                //PrintLog(historicalMessages->at(0)->first+" "+historicalMessages->at(0)->second);
                SendData(historicalMessages->at(0)->first, 2, historicalMessages->at(0)->second, user, userAvatars->at(userNames->indexOf(historicalMessages->at(0)->second)), 0);
            }*/
            if(hisMsgContent->size()>0)
            {
                SendData(hisMsgContent->at(0), 2, hisMsgSource->at(0), user, userAvatars->at(userNames->indexOf(hisMsgSource->at(0))), 0);
            }
            else
            {
                PrintLog(ShowIP(user)+" ("+remark+") received all historical messages");
                SendData("", 3, "", user);
            }
            break;
        case 5://Request historical message
            /*if(message.toInt()<historicalMessages->size())
            {
                SendData(historicalMessages->at(message.toInt())->first, 2, historicalMessages->at(message.toInt())->second, user, userAvatars->at(userNames->indexOf(historicalMessages->at(message.toInt())->second)), message.toInt());
            }*/
            if(message.toInt()<hisMsgContent->size())
            {
                SendData(hisMsgContent->at(message.toInt()), 2, hisMsgSource->at(message.toInt()), user, userAvatars->at(userNames->indexOf(hisMsgSource->at(message.toInt()))), message.toInt());
            }
            else
            {
                PrintLog(ShowIP(user)+" ("+remark+") received all historical messages");
                SendData("", 3, "", user);
            }
            break;
    }
    //PrintLog(user->readAll().toStdString().data());
}

void MessageServer::SaveData(QString path, QVector<QString>* data)
{
    QFile file(path);
    if(file.open(QIODevice::WriteOnly))
    {
        QJsonArray arr;
        for(int i=0; i<data->length(); i++)
        {
            arr.append(QJsonValue(data->at(i)));
        }
        QJsonDocument doc(arr);
        file.write(doc.toJson());
        file.close();
    }
}

QVector<QString>* MessageServer::LoadData(QString path)
{
    QVector<QString>* data = new QVector<QString>();
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray byteArr = file.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(byteArr));
        QJsonArray arr = doc.array();
        for (const QJsonValue &value : arr) {
            data->append(value.toString());
        }
        file.close();
    }
    return data;
}

void MessageServer::CreateJsonFile(QString path)
{
    QFile file(path);
    if(!file.exists())
    {
        if(file.open(QIODevice::WriteOnly))
        {
            QJsonArray jsonArr;
            QJsonDocument jsonDoc(jsonArr);
            QString jsonStr = jsonDoc.toJson(QJsonDocument::Indented);
            file.write(jsonStr.toUtf8());
        }
    }
}
