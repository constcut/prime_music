QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Experiments.cpp \
        NoteOperations.cpp \
        PrimeMusic.cpp \
        libs/sf/tsf.cpp \
        main.cpp \
        midi/MidiFile.cpp \
        midi/MidiMessage.cpp \
        midi/MidiRender.cpp \
        midi/MidiTrack.cpp \
        midi/NBytesInt.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Experiments.hpp \
    NoteOperations.hpp \
    PrimeMusic.hpp \
    libs/sf/tml.h \
    libs/sf/tsf.h \
    log.hpp \
    midi/MidiFile.hpp \
    midi/MidiMessage.hpp \
    midi/MidiRender.hpp \
    midi/MidiTrack.hpp \
    midi/MidiUtils.hpp \
    midi/NBytesInt.hpp

DISTFILES += \
    libs/sf/LICENSE \
    libs/sf/README.md
