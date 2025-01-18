#include "client_widget.h"
#include "ui_client_widget.h"

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWidget)
{
    this->setWindowTitle("聊天室");
    connectState = 0;//Offline
    ui->setupUi(this);
    ui->emojiButton->setText("表情");
    ui->messageButton->setText("发送");
    ui->nameButton->setText("修改昵称");
    ui->passwordButton->setText("修改密码");
    ui->connectButton->setText("登录/注册");
    ui->disconnectButton->setText("登出");
    ui->avatarButton->setText("修改头像");
    ui->backButton->setText("返回");
    ui->avatar->setText("");
    ui->ipEdit->setText("127.0.0.1");//Default ip
    ui->portEdit->setText("8000");//Default port
    ui->chatBox->setWidgetResizable(false);//Show scroll bar
    ui->chatWidget->setFixedHeight(0);
    ui->chatWidget->setLayout(ui->chatLayout);
    defaultAvatar.load(":/new/img/default.png");
    currentAvatar = defaultAvatar.copy();
    ui->avatar->setPixmap(defaultAvatar.scaled(QSize(60, 60)));
    //ui->messageEdit->setFont(emojiFont);

    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(ConnectSlot()));
    connect(ui->emojiButton, SIGNAL(clicked(bool)), this, SLOT(ShowEmojiSlot()));
    connect(ui->avatarButton, SIGNAL(clicked(bool)), this, SLOT(SelectAvatarSlot()));
    connect(ui->messageEdit, SIGNAL(textChanged()), this, SLOT(MessageChangedSlot()));
    connect(ui->backButton, SIGNAL(clicked(bool)), this, SLOT(JumpToMainMenuSlot()));

    //Init emoji table
    for(int i=0; i<25; i++)
    {
        QPushButton* emoji = new QPushButton(this);
        emoji->setText(emojis[i]);
        emoji->setFixedSize(30,30);
        emoji->setFont(emojiFont);
        emoji->hide();
        connect(emoji, SIGNAL(clicked(bool)), this, SLOT(InsertEmojiSlot()));
        ui->emojiLayout->addWidget(emoji, i/5, i%5);
    }
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

//Send data to server
void ClientWidget::SendData(QString msg, int type, QString rm)
{
    QJsonObject msgObj;
    msgObj["message"] = msg.toUtf8().toStdString().data();
    msgObj["type"] = type;//QString::number(type);
    msgObj["remark"] = rm;
    QJsonDocument jsonDoc(msgObj);
    socket->write(jsonDoc.toJson());
}

void ClientWidget::ChangeNameSlot()
{
    if(connectState == 2)//Login
    {
        //qDebug()<<ui->nameEdit->text();
        SendData(ui->nameEdit->text(), 1, name);
    }
}

void ClientWidget::ConnectSlot()
{
    if(ui->nameEdit->text() == "" || ui->passwordEdit->text() == "")
    {
       qDebug()<<"请输入用户名和密码";
    }
    else
    {
        name = ui->nameEdit->text();
        //Try to connect
        if(connectState>0)//Online
        {
            socket->disconnectFromHost();
        }
        socket = new QTcpSocket();
        socket->connectToHost((ui->ipEdit->text()).toStdString().data(), (ui->portEdit->text()).toInt());
        connect(socket, SIGNAL(connected()), this, SLOT(ConnectedSlot()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(DisconnectedSlot()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(ReadyReadSlot()));
        connect(ui->messageButton, SIGNAL(clicked(bool)), this, SLOT(SendMessageSlot()));
        connect(ui->nameButton, SIGNAL(clicked(bool)), this, SLOT(ChangeNameSlot()));
        connect(ui->disconnectButton, SIGNAL(clicked(bool)), this, SLOT(DisconnectSlot()));
    }
}

void ClientWidget::ConnectedSlot()
{
    //Connected to server
    connectState = 1;//Online
    //ui->nameEdit->setText((socket->localAddress()).toString()+":"+QString::number(socket->localPort()).toStdString().data());
    qDebug()<<(socket->localAddress()).toString()+":"+QString::number(socket->localPort())<<"Successfully connected to server";

    //Try to login
    SendData(ui->nameEdit->text(), -1, ui->passwordEdit->text());
}

void ClientWidget::DisconnectSlot()
{
    socket->disconnectFromHost();
}

void ClientWidget::DisconnectedSlot()
{
    currentAvatar = defaultAvatar.copy();
    ui->avatar->setPixmap(defaultAvatar.scaled(QSize(60, 60)));
    connectState = 0;//Offline
    qDebug()<<"Disconnected from server";
}

void ClientWidget::ReadyReadSlot()
{
    QTcpSocket* server = (QTcpSocket*)QObject::sender();
    QJsonObject msgObj = (QJsonDocument::fromJson(server->readAll())).object();
    QString message = msgObj["message"].toString();
    QString remark = msgObj["remark"].toString();
    qDebug()<<msgObj["type"].toInt();
    QList<QWidget*> oldMessages = ui->chatWidget->findChildren<QWidget*>("msgAlign");
    switch(msgObj["type"].toInt())
    {
        case -2://Register successfully
            connectState = 2;
            messageAvatar = new QVector<QLabel*>();
            messageSource = new QVector<QString>();
            qDebug()<<"messages:"<<oldMessages.count();
            for(int i=0; i<oldMessages.count(); i++)
            {
                oldMessages.at(i)->setParent(NULL);
                ui->chatLayout->removeWidget(oldMessages.at(i));
                delete oldMessages.at(i);
            }
            ui->chatWidget->setFixedHeight(0);
            SendImage(defaultAvatar, 4);
            break;
        case -1://Login successfully
            connectState = 2;//Login
            name = message;
            currentAvatar = LoadPngImage(remark);
            ui->avatar->setPixmap(currentAvatar);
            messageAvatar = new QVector<QLabel*>();
            messageSource = new QVector<QString>();
            qDebug()<<"messages:"<<oldMessages.count();
            for(int i=0; i<oldMessages.count(); i++)
            {
                oldMessages.at(i)->setParent(NULL);
                ui->chatLayout->removeWidget(oldMessages.at(i));
                delete oldMessages.at(i);
            }
            ui->chatWidget->setFixedHeight(0);
            SendData("0", 5);
            break;
        case 0://Receive message
            NewMessage(message, remark, msgObj["avatar"].toString());
            SendData(message, 2);
            break;
        case 1://Someone changed avatar
            qDebug()<<"remark: "<<remark<<", name:"<<name;
            if(remark == name)
            {
                currentAvatar = LoadPngImage(message);
                ui->avatar->setPixmap(currentAvatar);
            }
            //Change avatars of historical chats
            for(int i = 0; i < messageSource->count(); i++)
            {
                if(messageSource->at(i) == remark)
                {
                    messageAvatar->at(i)->setPixmap(LoadPngImage(message).scaled(heightPreset, heightPreset));
                }
            }
            break;
        case 2://Receive historical messages
            //qDebug()<<message.at(0)<<" "<<QString::number(((QString)message.at(0)).toInt()+1);
            SendData(QString::number(msgObj["extra"].toInt()+1), 5, name);
            if(msgObj["extra"].toInt()>=messageSource->size())
            {
                NewMessage(message, remark, msgObj["avatar"].toString());
            }
            else
            {
                messageSource->replace(msgObj["extra"].toInt(), remark);
                messageAvatar->at(msgObj["extra"].toInt())->setPixmap(LoadPngImage(msgObj["avatar"].toString()).scaled(heightPreset, heightPreset));
            }
            break;
        case 3://No more historical messages
            qDebug()<<"Received all historical messages.";
            break;
        case 4://Someone changed name
            if(remark == name){
                name = message;
            }
            for(int i=0; i<messageSource->size(); i++)
            {
                if(messageSource->at(i) == remark)
                {
                    messageSource->replace(i, message);
                }
            }
            break;
        case 5://Failed to change name
            break;
    }
}

//Send message to server
void ClientWidget::SendMessageSlot()
{
    if(connectState == 2 && !(ui->messageEdit->toPlainText().isEmpty()))
    {
        SendData(ui->messageEdit->toPlainText(), 0, name);
        ui->messageEdit->clear();
    }
}


void ClientWidget::NewMessage(QString msgText, QString username, QString msgAvatar)
{
    QLabel* avatar = new QLabel();
    QTextEdit* msgContent = new QTextEdit();
    QHBoxLayout* msgLayout = new QHBoxLayout();
    QString text = "";
    //msgContent->setFont(emojiFont);
    avatar->setFixedSize(heightPreset, heightPreset);
    avatar->setPixmap(LoadPngImage(msgAvatar).scaled(heightPreset, heightPreset));
    //Auto linefeed for excessively long text
    int count = 0;
    int maxLength = 0;
    int msgHeight = heightPreset;
    bool haveFullWidthChar = false;
    for(int i=0; i<msgText.length(); i++)
    {
        if(msgText.at(i) != '\n')
        {
            //Determine half-width or full-width char
            haveFullWidthChar = haveFullWidthChar ? true:(msgText.at(i).unicode()>127);
            count = (msgText.at(i).unicode()<128) ? count+1:count+2;

            //qDebug()<<haveFullWidthChar;
            if(count>maxCharInOneRow)
            {
                //Add linefeed
                msgHeight += haveFullWidthChar ? heightOffsetFullWidth:heightOffset;
                haveFullWidthChar = false;
                maxLength = (count>maxLength) ? count:maxLength;
                text.append('\n');
                count = 0;
            }
            text.append(msgText.at(i));
        }
        else
        {
            //Normal linefeed
            msgHeight += haveFullWidthChar ? heightOffsetFullWidth:heightOffset;
            haveFullWidthChar = false;
            maxLength = (count>maxLength) ? count:maxLength;
            count = 0;
            text.append('\n');
        }
    }
    SetEmojiFont(msgContent);
    maxLength = (maxLength==0) ? count:maxLength;
    msgHeight += haveFullWidthChar ? heightOffsetFullWidth:heightOffset;
    if(username == name)
    {
        msgLayout->addWidget(msgContent);
        msgLayout->addWidget(avatar);
    }
    else
    {
        msgLayout->addWidget(avatar);
        msgLayout->addWidget(msgContent);
    }
    msgContent->setText(text);
    msgContent->setReadOnly(true);
    QWidget* msg = new QWidget;
    QWidget* msgAlign = new QWidget;
    msgAlign->setObjectName("msgAlign");
    QHBoxLayout* msgAlignLayout = new QHBoxLayout();
    msgAlign->setLayout(msgAlignLayout);
    msgAlignLayout->addWidget(msg);
    qDebug()<<"Width:"<<widthPreset+maxLength*widthOffset<<"Height:"<<msgHeight;
    msg->setFixedSize(widthPreset+maxLength*widthOffset+heightPreset, msgHeight);
    msg->setLayout(msgLayout);
    qDebug()<<username<<" "<<name;
    if(username == name)
    {
        msgContent->setStyleSheet("background-color: rgb(20,255,20);");
        msgAlignLayout->setAlignment(Qt::AlignRight);
    }
    else
    {
        //msgContent->setStyleSheet("background-color: rgb(20,255,20);");
        msgAlignLayout->setAlignment(Qt::AlignLeft);
    }
    ui->chatLayout->addWidget(msgAlign);
    ui->chatBox->ensureWidgetVisible(msgAlign);
    ui->chatWidget->setFixedHeight(ui->chatWidget->height()+msgHeight+msgMargin);
    //Scroll to bottom
    ui->chatBox->verticalScrollBar()->setValue(ui->chatBox->verticalScrollBar()->maximum());
    //qDebug()<<"Height: "<<ui->chatWidget->height();
    messageSource->append(username);
    messageAvatar->append(avatar);
}

//Show emoji table
void ClientWidget::ShowEmojiSlot()
{
    for(int i=0; i<25; i++)
    {
        ui->emojiLayout->itemAt(i)->widget()->show();
    }
}
//Hide emoji table
void ClientWidget::HideEmoji()
{
    for(int i=0; i<25; i++)
    {
        ui->emojiLayout->itemAt(i)->widget()->hide();
    }
}

//Select avatar file
void ClientWidget::SelectAvatarSlot()
{
    //Desktop path
    QString path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString selectedImage = QFileDialog::getOpenFileName(this, "选择头像", path, "PNG Images(*.png);;");
    qDebug()<<selectedImage;
    if(!selectedImage.isEmpty())
    {
        selectedAvatar.load(selectedImage);
    }
    else
    {
        selectedAvatar = currentAvatar.copy();
    }
    if(connectState == 2)//Login
    {
        SendImage(selectedAvatar, 3);
    }
}
void ClientWidget::SendImage(QPixmap img, int type)
{
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    QPixmap standardAvatar =img.scaled(QSize(60, 60));
    standardAvatar.save(&buffer, "png");
    SendData(imageData.toBase64().toStdString().data(), type, name);
    //qDebug()<<"img:"<<imageData.toBase64().toStdString().data()<<'\n';
}
//Convert base64 string to QPixmap
QPixmap ClientWidget::LoadPngImage(QString imgBase64)
{
    QPixmap img;
    img.loadFromData(QByteArray::fromBase64(imgBase64.toLocal8Bit()), "png");
    return img;
}

//Insert emoji to message
void ClientWidget::InsertEmojiSlot()
{
    QPushButton* emoji = (QPushButton*)QObject::sender();
    ui->messageEdit->setText(ui->messageEdit->toPlainText()+emoji->text());
}

//Change font of emoji char
void ClientWidget::SetEmojiFont(QTextEdit* textEdit)
{
    /*QString str = textEdit->toPlainText();
    int length = str.length();
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat format;// = cursor.charFormat();
    int initPos = cursor.position();
    cursor.movePosition(QTextCursor::Start);
    format.setFontFamily("Segoe UI Emoji");
    for(int i=0; i<length; i++)
    {
        if(str.at(i).unicode()>127)//Special char
        {
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, i);
            cursor.setCharFormat(format);
            cursor.movePosition(i+1, QTextCursor::MoveAnchor);
        }
    }
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, initPos);
    cursor.clearSelection();
    textEdit->setTextCursor(cursor);*/
}
void ClientWidget::MessageChangedSlot()
{
    SetEmojiFont(ui->messageEdit);
}

void ClientWidget::JumpToMainMenuSlot()
{
    emit JumpToMainMenu();
    this->hide();
}
void ClientWidget::ShowClientSlot()
{
    this->show();
}

const QString ClientWidget::emojis[] = {
    "🙂",
    "😁",
    "😃",
    "😆",
    "😉",
    "😊",
    "😋",
    "😍",
    "😎",
    "😵",
    "😑",
    "😒",
    "😓",
    "😘",
    "😛",
    "😜",
    "🙁",
    "😞",
    "😠",
    "😢",
    "👍",
    "👌",
    "👎",
    "👏",
    "🤝"
};

const QFont ClientWidget::emojiFont = *(new QFont("Segoe UI Emoji"));
