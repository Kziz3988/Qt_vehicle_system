#include "main_menu.h"
#include "weather_widget.h"
#include "client_widget.h"
#include "mainwindow.h"
#include "MusicWidget.h"
#include "message_server.h"
#include "browser.h"
#include "browserwindow.h"
#include "videowidget.h"

#include <QApplication>
#include <QMainWindow>

//获取URL函数
QString getCommandLineUrlArgument()
{

    const QStringList args = QCoreApplication::arguments();
    if (args.count() > 1) {
        const QString lastArg = args.last();
        const bool isValidUrl = QUrl::fromUserInput(lastArg).isValid();
        if (isValidUrl)
            return lastArg;
    }
    return QString();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu main;
    WeatherWidget weather;
    ClientWidget client;
    client.setWindowTitle("即时通讯");
    MessageServer server;
    MainWindow image;
    MusicWidget music;
    VideoWidget video;

    //ClientWidget client2;
    //client2.setWindowTitle("即时通讯");

    // Open a directory dialog to let the user choose a folder
    //QString directory = QFileDialog::getExistingDirectory(nullptr, "Select Directory", "/home/user");
    QString directory = "/home/user/Photo";
    if (!directory.isEmpty()) {
        image.setDirectory(directory);
    }

    //设置任务栏中的图标
    a.setWindowIcon(QIcon(QLatin1String(":simplebrowser.svg")));
    //实例化一个窗口类
    BrowserWindow *window = new BrowserWindow();
    //添加一个窗口，静态函数instance()返回指向自己的指针
    //addWindow()在browser.cpp中定义
    Browser::instance().addWindow(window);


    //获取url,有就跳转，没有就跳转到home页
    const QString url = getCommandLineUrlArgument();
    //如果URl不为空
    if (!url.isEmpty())
        window->loadPage(url);
    else
        //默认首页，函数在browserwindows.cpp
        window->loadHomePage();
    window->hide();

    main.show();
    QObject::connect(&main, SIGNAL(JumpToWeather()), &weather, SLOT(ShowWeatherSlot()));
    QObject::connect(&weather, SIGNAL(JumpToMainMenu()), &main, SLOT(ShowMainMenuSlot()));
    QObject::connect(&main, SIGNAL(JumpToMessage()), &client, SLOT(ShowClientSlot()));
    QObject::connect(&client, SIGNAL(JumpToMainMenu()), &main, SLOT(ShowMainMenuSlot()));
    QObject::connect(&main, SIGNAL(JumpToImage()), &image, SLOT(ShowImageSlot()));
    QObject::connect(&image, SIGNAL(JumpToMainMenu()), &main, SLOT(ShowMainMenuSlot()));
    QObject::connect(&main, SIGNAL(JumpToMusic()), &music, SLOT(ShowMusicSlot()));
    QObject::connect(&music, SIGNAL(JumpToMainMenu()), &main, SLOT(ShowMainMenuSlot()));
    QObject::connect(&main, SIGNAL(JumpToVideo()), &video, SLOT(ShowVideoSlot()));
    QObject::connect(&video, SIGNAL(JumpToMainMenu()), &main, SLOT(ShowMainMenuSlot()));
    QObject::connect(&main, SIGNAL(JumpToExplorer()), &*window, SLOT(ShowExplorerSlot()));
    QObject::connect(&*window, SIGNAL(JumpToMainMenu()), &main, SLOT(ShowMainMenuSlot()));

    //QObject::connect(&main, SIGNAL(JumpToMessage()), &client2, SLOT(ShowClientSlot()));
    //QObject::connect(&client2, SIGNAL(JumpToMainMenu()), &main, SLOT(ShowMainMenuSlot()));

    return a.exec();
}
