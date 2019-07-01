QT += core
QT -= gui
LIBS += -lwt -lwthttp -lboost_signals  -lpqxx -lpq -lopencv_highgui
LIBS += -lopencv_core -lzbar -lopencv_imgproc -lopencv_imgcodecs
LIBS +=-lxlnt -lpodofo -lcrypto -lz  -ljpeg -lfontconfig -lfreetype
INCLUDEPATH=-Ixlnt/include
CONFIG += c++17
QMAKE_CXXFLAGS+= -std=c++17

TARGET = agroquimicos
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    pg_model.cpp \
    table_show.cpp \
    helloapplication.cpp \
    stock_list.cpp \
    stock_add.cpp \
    product_add.cpp \
    create_tabla.cpp \
    stock_add_bar.cpp \
    rest_service.cpp \
    blob_data.cpp \
    excel_export.cpp \
    export_pdf.cpp \
    formularios.cpp \
    coordenadas_dialog.cpp \
    table_add_data.cpp \
    auto_complete_wline.cpp


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    pg_model.h \
    table_show.h \
    helloapplication.h \
    stock_list.h \
    stock_add.h \
    product_add.h \
    create_tabla.h \
    stock_add_bar.h \
    rest_service.h \
    blob_data.h \
    excel_export.h \
    export_pdf.h \
    formularios.h \
    coordenadas_dialog.h \
    table_add_data.h \
    auto_complete_wline.h

