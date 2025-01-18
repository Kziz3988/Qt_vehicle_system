#include "weather_widget.h"
#include "ui_weather_widget.h"

WeatherWidget::WeatherWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WeatherWidget)
{
    ui->setupUi(this);
    InitResource();
    this->setWindowTitle("天气预报");
    for(int i=0; i<18; i++)
    {
        labels[i] = new QLabel(this);
    }
    cityInput =new QLineEdit(this);
    cityInput->setGeometry(0, 0, 400, 50);
    cityInput->show();
    QPushButton* submit = new QPushButton(this);
    submit->setGeometry(400, 0, 100, 50);
    submit->setText("查询");
    connect(submit, SIGNAL(clicked()), this, SLOT(RequestSlot()));
    GetWeather("北京", 0, 3);
    backButton->setText("返回");
    backButton->setGeometry(500, 0, 100, 50);
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(JumpToMainMenuSlot()));
}

WeatherWidget::~WeatherWidget()
{
    delete ui;
}

//Get 3-day weather data
void WeatherWidget::GetWeather(QString location = "beijing", int a = 0, int b = 3)
{
    QNetworkAccessManager* nwm = new QNetworkAccessManager(this);
    QNetworkRequest nwr;
    dateStart = a;
    dateEnd = b;
    city = location;

    //Send request to seniverse api
    nwr.setUrl(QUrl("https://api.seniverse.com/v3/weather/daily.json?key=ScOyRQp3ykhBlHeuJ&location="+location+"&language=en&unit=c"));
    QNetworkReply* reply = nwm->get(nwr);
    connect(reply, SIGNAL(finished()), this, SLOT(ReplySlot()));
}

void WeatherWidget::ReplySlot()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    QTextCodec* codec = QTextCodec::codecForName("gbk");//Support Chinese
    QString replyData = codec->toUnicode(reply->readAll());
    reply->deleteLater();
    //qDebug("%s", replyData.toStdString().data());
    //qDebug("%s", " ");
    if(replyData.contains("status"))//Invalid request
    {
        tempHigh = "未知";
        tempLow = "未知";
        weatherDay = "99";
        weatherNight = "99";
        for(int i=dateStart; i<dateEnd; i++)
        {
            ShowLabels(i);
        }
    }
    else{
        //Find data of requesting date
        for(int date = dateStart; date < dateEnd; date++){
            int start = replyData.indexOf("date")+7;
            int end = replyData.indexOf("text_day")-3;
            today = QDate::fromString(replyData.mid(start, end-start).toStdString().data(), "yyyy-MM-dd");
            QString dateData = replyData.mid(replyData.indexOf(today.addDays(date).toString("yyyy-MM-dd")));
            qDebug("%s", dateData.toStdString().data());
            qDebug("%s", " ");

            //Get city name
            /*start = replyData.indexOf("name")+7;
            end = replyData.indexOf("country")-3;
            city = replyData.mid(start, end-start);
            qDebug("%s", city.toStdString().data());*/

            //Get weather
            start = dateData.indexOf("code_day")+11;
            end = dateData.indexOf("text_night")-3;
            weatherDay = dateData.mid(start, end-start);
            qDebug("%s", weatherDay.toStdString().data());
            start = dateData.indexOf("code_night")+13;
            end = dateData.indexOf("high")-3;
            weatherNight = dateData.mid(start, end-start);
            qDebug("%s", weatherNight.toStdString().data());

            //Get temperature
            start = dateData.indexOf("high")+7;
            end = dateData.indexOf("low")-3;
            tempHigh = dateData.mid(start, end-start)+"℃";
            qDebug("%s", tempHigh.toStdString().data());
            start = dateData.indexOf("low")+6;
            end = dateData.indexOf("rainfall")-3;
            tempLow = dateData.mid(start, end-start)+"℃";
            qDebug("%s", tempLow.toStdString().data());

            ShowLabels(date);
        }
    }

    //Show city info
    ShowText(labels[15], city, *weatherFont, 10, 50, 1000 ,100);
    ShowText(labels[16], today.toString("yyyy-MM-dd"), *dayFont, 10, 100, 200 ,100);
    ShowText(labels[17], dayName[today.dayOfWeek()], *dayFont, 210, 100, 200 ,100);
}

void WeatherWidget::RequestSlot()
{
    GetWeather(cityInput->text(), 0, 3);
}

//Show weather info
void WeatherWidget::ShowLabels(int date)
{
    //Show weather icon
    //Q_INIT_RESOURCE(img);
    labels[date*5]->setGeometry(10+date*200, 150, 100, 100);
    labels[date*5+1]->setGeometry(10+date*200, 300, 100, 100);
    QString weatherPicPath = ":/new/img/"+weatherDay+"@2x.png";
    labels[date*5]->setPixmap(QPixmap(weatherPicPath));
    weatherPicPath = ":/new/img/"+weatherNight+"@2x.png";
    labels[date*5+1]->setPixmap(QPixmap(weatherPicPath));
    //qDebug("%s", weatherPicPath.toStdString().data());
    labels[date*5]->show();
    labels[date*5+1]->show();

    //Show temperature
    ShowText(labels[date*5+2], tempHigh, *weatherFont, 70+date*200, 150, 150, 100);
    ShowText(labels[date*5+3], tempLow, *weatherFont, 70+date*200, 300, 150, 100);

    //Show day
    ShowText(labels[date*5+4], dayName[today.addDays(date).dayOfWeek()], *dayFont, 20+date*200, 400, 100, 100);
}

//Show label text
void WeatherWidget::ShowText(QLabel* label, QString text, QFont font, int x, int y, int w, int h)
{
    //qDebug("%s", text.toStdString().data());
    label->setText(text);
    label->setFont(font);
    label->setGeometry(x, y, w, h);
    label->show();
}

void WeatherWidget::InitResource()
{
    Q_INIT_RESOURCE(img);
}
void WeatherWidget::ShowWeatherSlot()
{
    this->show();
}
void WeatherWidget::JumpToMainMenuSlot()
{
    emit JumpToMainMenu();
    this->hide();
}

const QString WeatherWidget::weatherText[] = {
    "晴",
    "晴",
    "晴",
    "晴",
    "多云",
    "晴间多云",
    "晴间多云",
    "大部多云",
    "大部多云",
    "阴",
    "阵雨",
    "雷阵雨",
    "冰雹",
    "小雨",
    "中雨",
    "暴雨",
    "大暴雨",
    "特大暴雨",
    "冻雨",
    "雨夹雪",
    "阵雪",
    "小雪",
    "中雪",
    "大雪",
    "暴雪",
    "浮尘",
    "扬沙",
    "沙尘暴",
    "强沙尘暴",
    "雾",
    "霾",
    "风",
    "大风",
    "飓风",
    "热带风暴",
    "龙卷风",
    "冷",
    "热",
    "未知"
};

const QString WeatherWidget::dayName[] = {
    "星期日",
    "星期一",
    "星期二",
    "星期三",
    "星期四",
    "星期五",
    "星期六",
    "星期日"
};

const QFont* WeatherWidget::weatherFont = new QFont("Arial", 50);
const QFont* WeatherWidget::dayFont = new QFont("Arial", 20);
