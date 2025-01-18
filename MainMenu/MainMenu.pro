QT       += core gui
QT       += network
QT += webenginewidgets
QT += multimedia
QT += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    main_menu.cpp

HEADERS += \
    main_menu.h

FORMS += \
    main_menu.ui

TRANSLATIONS += \
    MainMenu_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
    weatherImg.qrc
    clientImg.qrc
    icon.qrc
    data/simplebrowser.qrc
    res.qrc
    videoImg.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../weather/release/ -lweather
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../weather/debug/ -lweather
else:unix: LIBS += -L$$OUT_PWD/../weather/ -lweather

INCLUDEPATH += $$PWD/../weather
DEPENDPATH += $$PWD/../weather

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ClientWidget/release/ -lclient_widget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ClientWidget/debug/ -lclient_widget
else:unix: LIBS += -L$$OUT_PWD/../ClientWidget/ -lclient_widget

INCLUDEPATH += $$PWD/../ClientWidget
DEPENDPATH += $$PWD/../ClientWidget

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MessageServer/release/ -lmessage_server
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MessageServer/debug/ -lmessage_server
else:unix: LIBS += -L$$OUT_PWD/../MessageServer/ -lmessage_server

INCLUDEPATH += $$PWD/../MessageServer
DEPENDPATH += $$PWD/../MessageServer

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../tupianapp/release/ -ltupianapp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../tupianapp/debug/ -ltupianapp
else:unix: LIBS += -L$$OUT_PWD/../tupianapp/ -ltupianapp

INCLUDEPATH += $$PWD/../tupianapp
DEPENDPATH += $$PWD/../tupianapp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../simplebrowser/simplebrowser/release/ -lsimplebrowser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../simplebrowser/simplebrowser/debug/ -lsimplebrowser
else:unix: LIBS += -L$$OUT_PWD/../simplebrowser/simplebrowser/ -lsimplebrowser

INCLUDEPATH += $$PWD/../simplebrowser/simplebrowser
DEPENDPATH += $$PWD/../simplebrowser/simplebrowser

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MusicPlayer/release/ -lmusic_player
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MusicPlayer/debug/ -lmusic_player
else:unix: LIBS += -L$$OUT_PWD/../MusicPlayer/ -lmusic_player

INCLUDEPATH += $$PWD/../MusicPlayer
DEPENDPATH += $$PWD/../MusicPlayer

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MessageServer/release/ -lmessage_server
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MessageServer/debug/ -lmessage_server
else:unix: LIBS += -L$$OUT_PWD/../MessageServer/ -lmessage_server

INCLUDEPATH += $$PWD/../MessageServer
DEPENDPATH += $$PWD/../MessageServer

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../VedioPlayer/release/ -lvideo_player
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../VedioPlayer/debug/ -lvideo_player
else:unix: LIBS += -L$$OUT_PWD/../VedioPlayer/ -lvideo_player

INCLUDEPATH += $$PWD/../VedioPlayer
DEPENDPATH += $$PWD/../VedioPlayer
