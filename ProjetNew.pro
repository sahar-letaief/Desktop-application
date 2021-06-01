QT       += core gui multimedia sql printsupport serialport charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alarmearduino.cpp \
    alarmeui.cpp \
    ambulance.cpp \
    arduinow.cpp \
    chambre.cpp \
    connectdb.cpp \
    employe.cpp \
    equipement.cpp \
    main.cpp \
    medicament.cpp \
    menu.cpp \
    mission.cpp \
    patient.cpp \
    rendezvous.cpp \
    reunion.cpp \
    service.cpp \
    smtp.cpp

HEADERS += \
    alarmearduino.h \
    alarmeui.h \
    ambulance.h \
    arduinow.h \
    chambre.h \
    connectdb.h \
    connection.h \
    employe.h \
    equipement.h \
    medicament.h \
    menu.h \
    mission.h \
    patient.h \
    rendezvous.h \
    reunion.h \
    service.h \
    smtp.h

FORMS += \
    alarmeui.ui \
    menu.ui \
    reunion.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pics.qrc

win32:RC_ICONS += logo.ico

DISTFILES += \
    pics/fleche.png \
    pics/modifier.png \
    pics/precedent.png \
    pics/printSahar.png \
    pics/search.png \
    pics/service.png \
    pics/sort.png \
    pics/stats.png \
    pics/suivant.png \
    pics/trashSahar.png \
    pics/valider.png
