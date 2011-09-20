#-------------------------------------------------
#
# Project created by QtCreator 2011-07-24T17:20:29
#
#-------------------------------------------------

QT       += core gui sql testlib

TARGET = eParts
TEMPLATE = app

PREFIX=F:\MyProjects\QtApps

#LIBS += -L$$PREFIX/lib/dquest -ldquest
#LIBS += -L$$PREFIX/lib/dquest -ldquest

include(F:\MyProjects\QtApps\include\dquest\dquest.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    partsmainwidget.cpp \
    types.cpp \
    partstablemodel.cpp \
    database.cpp \
    unitformatter.cpp \
    test1.cpp

HEADERS  += mainwindow.h \
    connection.h \
    partsmainwidget.h \
    constants.h \
    types.h \
    partstablemodel.h \
    database.h \
    category.h \
    parttype.h \
    partparameter.h \
    part.h \
    parametervalue.h \
    mockdata.h \
    unitformatter.h \
    AutoTest.h \
    test1.h

FORMS    += mainwindow.ui \
    partsmainwidget.ui

RESOURCES += \
    resources.qrc



