#-------------------------------------------------
#
# Project created by QtCreator 2020-12-17T09:10:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EnergySegmetationOfImageProject
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -LD:/OpenCV/OpenCV_4_5_0/opencv/open_cv_build_msvc/lib/Release/ -lopencv_core450
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OpenCV/OpenCV_4_5_0/opencv/open_cv_build_msvc/lib/Debug/ -lopencv_core450d

win32:CONFIG(release, debug|release): LIBS += -LD:/OpenCV/OpenCV_4_5_0/opencv/open_cv_build_msvc/lib/Release/ -lopencv_imgproc450
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OpenCV/OpenCV_4_5_0/opencv/open_cv_build_msvc/lib/Debug/ -lopencv_imgproc450d

win32:CONFIG(release, debug|release): LIBS += -LD:/OpenCV/OpenCV_4_5_0/opencv/open_cv_build_msvc/lib/Release/ -lopencv_imgcodecs450
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OpenCV/OpenCV_4_5_0/opencv/open_cv_build_msvc/lib/Debug/ -lopencv_imgcodecs450d

win32:CONFIG(release, debug|release): LIBS += -LD:/OpenCV/OpenCV_4_5_0/opencv/open_cv_build_msvc/lib/Release/ -lopencv_highgui450
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OpenCV/OpenCV_4_5_0/opencv/open_cv_build_msvc/lib/Debug/ -lopencv_highgui450d

INCLUDEPATH += D:/OpenCV/OpenCV_4_5_0/opencv/open_cv_build_msvc/install/include
DEPENDPATH += D:/OpenCV/OpenCV_4_5_0/opencv/open_cv_build_msvc/install/include

RESOURCES += \
    icons.qrc
