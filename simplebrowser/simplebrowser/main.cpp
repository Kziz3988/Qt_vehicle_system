#include <QMainWindow>
#include "browser.h"
#include "browserwindow.h"
#include <QApplication>

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

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    //设置任务栏中的图标
    app.setWindowIcon(QIcon(QLatin1String(":simplebrowser.svg")));
    //实例化一个窗口类
    BrowserWindow *window = new BrowserWindow();
    //添加一个窗口，静态函数instance()返回指向自己的指针
    //addWindow()在browser.cpp中定义
    Browser::instance().addWindow(window);


    //获取url,有就跳转，没有就天赚到home页
    const QString url = getCommandLineUrlArgument();
    //如果URl不为空
    if (!url.isEmpty())
        window->loadPage(url);
    else
        //默认首页，函数在browserwindows.cpp
        window->loadHomePage();

    return app.exec();
}
