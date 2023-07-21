QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    autonom.cpp \
    main.cpp \
    mainwindow.cpp \
    mapping.cpp \
    myvideocapture.cpp


RESOURCES += \
    resources.qrc


HEADERS += \
    autonom.h \
    mainwindow.h \
    mapping.h \
    util.h \
    myvideocapture.h

FORMS += \
    mainwindow.ui

# Modify the following path according to your OpenCV installation
INCLUDEPATH += C:\Users\Hasan\Downloads\opencv\build\include
DEPENDPATH += C:\Users\Hasan\Downloads\opencv\build\include


# Link against OpenCV libraries
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_gapi480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_objdetect480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_video480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_dnn480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_stitching480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_calib3d480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_highgui480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_videoio480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_features2d480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_imgcodecs480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_photo480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_imgproc480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_core480.dll
LIBS += C:\Users\Hasan\Downloads\opencv\build\install\x64\mingw\bin\libopencv_ml480.dll



# If you are on Windows and using the WinSock library, uncomment the following line
LIBS += -lws2_32


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
