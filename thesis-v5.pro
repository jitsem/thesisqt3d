TEMPLATE = app
CONFIG += c++11

QT+=qml quick widgets 3dcore 3dinput 3drender 3dquick xml svg quickwidgets

QTPLUGIN += qsvg@

SOURCES += main.cpp \
    resistor.cpp \
    source.cpp \
    calc.cpp \
    component.cpp \
    wire.cpp \
    switch.cpp \
    component_lb.cpp \
    dragcomponent.cpp \
    drawzone.cpp \
    mainwindow.cpp

HEADERS += \
    calc.h \
    resistor.h \
    source.h \
    component.h\
    wire.h \
    switch.h \
    component_lb.h \
    dragcomponent.h \
    drawzone.h \
    mainwindow.h



DISTFILES += \
    solution.png \
    Qml/InputController.qml \
    Qml/World3D.qml \
    Qml/Source.qml \
    Qml/Generator.qml \
    Qml/Resistor.qml \
    Qml/GroundPlane.qml \
    Qml/Wire.qml \
    Qml/Electron.qml \
    Qml/CircuitView.qml \
    Qml/EditMenu.qml \
    Qml/Switch.qml \
    \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

RESOURCES += \
    qml.qrc \
    resource.qrc \
    levelbuilder.qrc \

INCLUDEPATH += $$PWD/Eigen/

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3

FORMS += \
    mainwindow.ui





