#-------------------------------------------------
#
# Project created by QtCreator 2011-07-24T17:20:29
#
#-------------------------------------------------

QT += core gui sql testlib

TARGET = eParts
TEMPLATE = app

win32:DQUEST_INCLUDE_PATH=$$quote(F:\MyProjects\QtApps\include\dquest\dquest.pri)
unix:DQUEST_INCLUDE_PATH=$$quote(/usr/local/include/dquest/dquest.pri)

include($$DQUEST_INCLUDE_PATH)

SOURCES += main.cpp\
        mainwindow.cpp \
    partsmainwidget.cpp \
    types.cpp \
    partstablemodel.cpp \
    database.cpp \
    unitformatter.cpp \
    test1.cpp \
    partmodel.cpp \
    qunitlineedit.cpp \
    partdetailswidget.cpp

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
    test1.h \
    partmodel.h \
    qunitlineedit.h \
    partdetailswidget.h

FORMS    += mainwindow.ui \
    partsmainwidget.ui

RESOURCES += \
    resources.qrc









