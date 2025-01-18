#include <QApplication>
#include "videowidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    VideoWidget window;
    window.show();

    return app.exec();
}
