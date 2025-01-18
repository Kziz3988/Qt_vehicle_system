#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setDirectory("/home/user/Photo");
    w.show();
    return a.exec();
}
