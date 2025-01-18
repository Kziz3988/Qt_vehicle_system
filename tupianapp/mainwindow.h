#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QScrollBar>
#include <QMouseEvent>
#include <QStatusBar>
#include <QVector>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setDirectory(const QString &directoryPath);

private:
    Ui::MainWindow *ui;
    int currentIndex;
    QPoint croppingStart;
       QPoint croppingEnd;
       QDir directory;
       QLabel *imageLabel;

       QScrollArea *scrollArea;

       QString fileName;
       QImage image;
       QStringList imageFiles;

       QStatusBar *statusBar;

       QToolBar *mainToolBar;

       QVector<QImage> history;
       QVector<QImage> reverseHistory;

       QAction *actionCrop;
       QAction *actionOpen;
       QAction *actionPaintBlack;
       QAction *actionRotateLeft;
       QAction *actionRotateRight;
       QAction *actionSave;
       QAction *actionShowToolbar;
       QAction *actionUndo;
       QAction *actionRedo;
       QAction *actionZoomIn;
       QAction *actionZoomOut;
       QAction *actionZoomToFit;
       QAction *actionLeft;
       QAction *actionRight;

       double scaleFactor;
       bool croppingState;

       void adjustScrollBar(QScrollBar *scrollBar, double factor);
       void changeCroppingState(bool changeTo);
       void refreshLabel();
       void rotateImage(int angle);
       void saveToHistory(QImage imageToSave);
       void saveToHistoryWithClear(QImage imageToSave);
       void saveToReverseHistory(QImage imageToSave);
       void scaleImage(double factor);
       void updateActions(bool updateTo);
public slots:
       void ShowImageSlot();
signals:
       void JumpToMainMenu();

private slots:
    bool eventFilter(QObject* watched, QEvent* event);

        void on_actionCrop_triggered();
        void on_actionFullscreen_triggered();
        void on_actionPaintBlack_triggered();
        void on_actionOpen_triggered();
        void on_actionRotateLeft_triggered();
        void on_actionRotateRight_triggered();
        void on_actionSave_triggered();
        void on_actionShowToolbar_triggered(bool checked);
        void on_actionUndo_triggered();
        void on_actionRedo_triggered();
        void on_actionZoomIn_triggered();
        void on_actionZoomOut_triggered();
        void on_actionZoomToFit_triggered();
        void on_actionLeft_triggered();
        void on_actionRight_triggered();

        void JumpToMainMenuSlot();
};
#endif // MAINWINDOW_H
