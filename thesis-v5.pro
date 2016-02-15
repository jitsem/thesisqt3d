TEMPLATE = app
CONFIG += console c++11

QT+=qml quick widgets 3dcore 3dinput 3drender 3dquick

SOURCES += main.cpp \
    resistor.cpp \
    source.cpp \
    calc.cpp \
    component.cpp \

HEADERS += \
    calc.h \
    resistor.h \
    source.h \
    component.h\



DISTFILES += \
    solution.png \
    Qml/AppView.qml \
    Qml/InputController.qml \
    Qml/World3D.qml \
    Qml/Source.qml \
    Qml/Generator.qml \
    Qml/Resistor.qml \
    Qml/GroundPlane.qml \
    Qml/Wire.qml \

RESOURCES += \
    qml.qrc \
    resource.qrc

INCLUDEPATH += $$PWD/Eigen/
