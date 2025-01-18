#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include <QPalette>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

public slots:
    void ShowMainMenuSlot();
signals:
    void JumpToWeather();
    void JumpToMessage();
    void JumpToImage();
    void JumpToMusic();
    void JumpToVideo();
    void JumpToExplorer();

private:
    Ui::MainMenu *ui;
    void SetButtonImage(QString, QPushButton*);

private slots:
    void JumpToWeatherSlot();
    void JumpToMessageSlot();
    void JumpToImageSlot();
    void JumpToMusicSlot();
    void JumpToVideoSlot();
    void JumpToExplorerSlot();
};
#endif // MAINMENU_H
