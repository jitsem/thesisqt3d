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
    drawing \
    Qml/AppView.qml \
    Qml/GameController.qml \
    Qml/InputController.qml \
    Qml/World3D.qml \
    Qml/Comp.qml

RESOURCES += \
    qml.qrc \
    resource.qrc

INCLUDEPATH += $$PWD/Eigen/
