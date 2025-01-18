TEMPLATE = app
TARGET = simplebrowser
QT += webenginewidgets
CONFIG += c++11
TEMPLATE = lib
DEFINES += simplebrowser_LIBRARY

HEADERS += \
    browser.h \
    browserwindow.h \
    correlation.h \
    tabwidget.h \
    urllineedit.h \
    webview.h \
    webpage.h \
    webpopupwindow.h

SOURCES += \
    browser.cpp \
    browserwindow.cpp \
    tabwidget.cpp \
    urllineedit.cpp \
    webview.cpp \
    webpage.cpp \
    webpopupwindow.cpp

FORMS += \
    certificateerrordialog.ui \
    passworddialog.ui

RESOURCES += data/simplebrowser.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/webenginewidgets/simplebrowser
INSTALLS += target
