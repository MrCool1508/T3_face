#-------------------------------------------------
#
# Project created by QtCreator 2017-12-14T14:33:38
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = T3_Face_Server
TEMPLATE = app


SOURCES += main.cpp\
    FaceWidget.cpp \
    FaceinfoDatabaseWidget.cpp \
    FaceEngine.cpp \
    AddNewFaceWidget.cpp \
    Decoder.cpp \
    RecordDatabaseWidget.cpp


HEADERS  += \
    t3_log.h \
    arcsoft_fsdk_face_recognition.h \
    arcsoft_fsdk_face_tracking.h \
    key.h \
    amcomdef.h \
    asvloffscreen.h \
    FaceWidget.h \
    FaceinfoDatabaseWidget.h \
    FaceEngine.h \
    Decoder.h \
    AddNewFaceWidget.h \
    RecordDatabaseWidget.h

FORMS    += \
    AddNewFaceWidget.ui \
    FaceinfoDatabaseWidget.ui \
    FaceWidget.ui \
    RecordDatabaseWidget.ui

unix:LIBS        +=  -L../T3_Face_Server -larcsoft_fsdk_face_tracking -larcsoft_fsdk_face_recognition -lxmnetsdk -lavcodec -lavutil -lswscale

