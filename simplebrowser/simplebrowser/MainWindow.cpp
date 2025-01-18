#include "MainWindow.h"
#include "browserwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), browserWindow(nullptr) {
    openBrowserButton = new QPushButton("Open Browser Window", this);
    connect(openBrowserButton, &QPushButton::clicked, this, &MainWindow::openBrowserWindow);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(openBrowserButton);
    setCentralWidget(centralWidget);
}

void MainWindow::openBrowserWindow() {
    if (!browserWindow) {
        browserWindow = new BrowserWindow(this); // Passing `this` as parent
        browserWindow->show();
    }
}

