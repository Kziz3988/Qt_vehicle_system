TEMPLATE = subdirs

SUBDIRS += \
    ClientWidget \
    MainMenu \
    MessageServer \
    MusicPlayer \
    VedioPlayer \
    simplebrowser/simplebrowser \
    tupianapp \
    weather

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/simplebrowser/simplebrowser/release/ -lsimplebrowser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/simplebrowser/simplebrowser/debug/ -lsimplebrowser
else:unix: LIBS += -L$$OUT_PWD/simplebrowser/simplebrowser/ -lsimplebrowser

INCLUDEPATH += $$PWD/simplebrowser/simplebrowser
DEPENDPATH += $$PWD/simplebrowser/simplebrowser

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/tupianapp/release/ -ltupianapp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/tupianapp/debug/ -ltupianapp
else:unix: LIBS += -L$$OUT_PWD/tupianapp/ -ltupianapp

INCLUDEPATH += $$PWD/tupianapp
DEPENDPATH += $$PWD/tupianapp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/MusicPlayer/release/ -lmusic_player
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/MusicPlayer/debug/ -lmusic_player
else:unix: LIBS += -L$$OUT_PWD/MusicPlayer/ -lmusic_player

INCLUDEPATH += $$PWD/MusicPlayer
DEPENDPATH += $$PWD/MusicPlayer
