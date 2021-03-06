QT += widgets opengl openglwidgets gui

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    glrenderer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CS1230Lib/resourceloader.h \
    glrenderer.h \
    mainwindow.h


# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += glm
DEPENDPATH += glm

DISTFILES += \
    Resources/Shaders/default.frag \
    Resources/Shaders/default.vert \
