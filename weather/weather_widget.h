#ifndef WEATHER_WIDGET_H
#define WEATHER_WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextCodec>
#include <QLabel>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>
#include <QFont>

QT_BEGIN_NAMESPACE
namespace Ui { class WeatherWidget; }
QT_END_NAMESPACE

class WeatherWidget : public QWidget
{
    Q_OBJECT

public:
    WeatherWidget(QWidget *parent = nullptr);
    ~WeatherWidget();

    void GetWeather(QString, int, int);
    void InitResource();

    QDate today;
    QString city, weatherDay, weatherNight, tempHigh, tempLow;
    static const QString weatherText[40];
    static const QString dayName[8];

public slots:
    void ShowWeatherSlot();
signals:
    void JumpToMainMenu();

private:
    Ui::WeatherWidget *ui;
    int dateStart, dateEnd;
    QLineEdit* cityInput;
    QLabel* labels[18];
    static const QFont* weatherFont;
    static const QFont* dayFont;
    QPushButton* backButton = new QPushButton(this);

    void ShowLabels(int);
    void ShowText(QLabel*, QString, QFont, int, int, int, int);

private slots:
    void ReplySlot();
    void RequestSlot();
    void JumpToMainMenuSlot();
};
#endif // WEATHER_WIDGET_H
