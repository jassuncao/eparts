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
    models/partstablemodel.cpp \
    database/database.cpp \
    unitformatter.cpp \
    test1.cpp \
    partmodel.cpp \
    widgets/qunitlineedit.cpp \
    partdetailswidget.cpp \
    settingsdialog.cpp \
    categoriesform.cpp \
    categorydialog.cpp \
    models/categorieslistmodel.cpp \
    parttypesform.cpp \
    models/parttypetablemodel.cpp \
    parttypedialog.cpp \
    models/partparameterslistmodel.cpp \
    models/partparameterstablemodel.cpp \
    models/parttypemodel.cpp \
    models/partstablerow.cpp \
    spinboxdelegate.cpp \
    widgets/qsearchlineedit.cpp \
    widgets/fancytabwidget.cpp \
    widgets/styledbar.cpp \
    widgets/stylehelper.cpp \
    unitparser.cpp \
    unitcolumndelegate.cpp \
    models/part.cpp \
    models/category.cpp \
    models/partattribute.cpp \
    models/parttablemodel.cpp \
    models/attributeformatter.cpp \
    partscontainerwidget.cpp \
    editpartdialog.cpp \
    widgets/flowlayout.cpp \
    selectattributedialog.cpp \
    editattributedialog.cpp \
    widgets/attributeeditorwidget.cpp \
    widgets/qlineedit2.cpp \
    models/attributesrepository.cpp \
    models/attributestablemodel.cpp


HEADERS  += mainwindow.h \
    connection.h \
    partsmainwidget.h \
    constants.h \
    types.h \
    models/partstablemodel.h \
    database/database.h \
    database/category.h \
    database/parttype.h \
    database/partparameter.h \
    database/part.h \
    parametervalue.h \
    mockdata.h \
    widgets/unitformatter.h \
    AutoTest.h \
    test1.h \
    partmodel.h \
    widgets/qunitlineedit.h \
    partdetailswidget.h \
    settingsdialog.h \
    categoriesform.h \
    categorydialog.h \
    models/categorieslistmodel.h \
    parttypesform.h \
    models/parttypetablemodel.h \
    modelrow.h \
    parttypedialog.h \
    models/partparameterslistmodel.h \
    models/partparameterstablemodel.h \
    models/parttypemodel.h \
    models/partstablerow.h \
    spinboxdelegate.h \
    widgets/qsearchlineedit.h \
    widgets/fancytabwidget.h \
    widgets/styledbar.h \
    widgets/stylehelper.h \
    unitparser.h \
    unitcolumndelegate.h \
    database/parametervalue.h \
    models/part.h \
    models/category.h \
    models/partattribute.h \
    models/parttablemodel.h \
    models/attributeformatter.h \
    database/entities.h \
    partscontainerwidget.h \
    editpartdialog.h \
    widgets/flowlayout.h \
    selectattributedialog.h \
    editattributedialog.h \
    widgets/attributeeditorwidget.h \
    widgets/qlineedit2.h \
    models/attributesrepository.h \
    models/attributestablemodel.h

FORMS    += mainwindow.ui \
    partsmainwidget.ui \
    settingsdialog.ui \
    categoriesform.ui \
    categorydialog.ui \
    parttypesform.ui \
    parttypedialog.ui \
    testwindow.ui \
    editpartdialog.ui \
    selectattributedialog.ui \
    editattributedialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    Todo.txt \
    create.sql
