QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 # c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    NormalizedVector3d.cpp \
    RotationTriple.cpp \
    SphereGravityModel.cpp \
    SphereMaterialPoint.cpp \
    SphereOrthogonalProjectionArcDrawer.cpp \
    SphereOrthogonalProjectionScene.cpp \
    Vector2d.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h \
    NormalizedVector3d.h \
    RotationTriple.h \
    SphereGravityModel.h \
    SphereMaterialPoint.h \
    SphereOrthogonalProjectionArcDrawer.h \
    SphereOrthogonalProjectionScene.h \
    Vector3d.h \
    Vector2d.h \
    Math.h \
    Vector3dDefinitions.h

FORMS += \
    MainWindow.ui

TRANSLATIONS += \
    GaussGravityProject_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
