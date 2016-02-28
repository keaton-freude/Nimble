#-------------------------------------------------
#
# Project created by QtCreator 2015-08-15T15:06:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = NimbleEditor
TEMPLATE = app

CONFIG += console


SOURCES += main.cpp\
        mainwindow.cpp \
    d3drenderwidget.cpp \
    fpscalc.cpp \
    vectorwidget.cpp \
    qcustomplot.cpp \
    ../NimbleGraphics/NimbleGraphics/src/ParticleSettings.cpp \
    ../NimbleGraphics/NimbleGraphics/src/ParticleShader.cpp \
    ../NimbleGraphics/NimbleGraphics/src/ParticleSystem.cpp \
    ../NimbleGraphics/NimbleGraphics/src/*.cpp

SOURCES += ..\NimbleGraphics\NimbleGraphics\src\*.cpp


HEADERS  += mainwindow.h \
    d3drenderwidget.h \
    fpscalc.h \
    vectorwidget.h \
    qcustomplot.h

#HEADERS += ..\NimbleGraphics\NimbleGraphics\include\*.h

INCLUDEPATH += ..\NimbleGraphics\NimbleGraphics\include
INCLUDEPATH += ..\Externals\directxtk\include

FORMS    += mainwindow.ui

LIBS += -l..\..\bin\NimbleGraphics
LIBS += -l..\..\Externals\directxtk\lib\x64\Debug\DirectXTK
LIBS += -luser32
