#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

class BrowserWindow; // Forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void openBrowserWindow();

private:
    BrowserWindow *browserWindow;
    QPushButton *openBrowserButton;
};

#endif // MAINWINDOW_H

