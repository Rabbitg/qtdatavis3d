!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}

SOURCES += main.cpp scatterdatamodifier.cpp \
    custominputhandler.cpp
HEADERS += scatterdatamodifier.h \
    custominputhandler.h

QT += widgets

INSTALLS += target

RESOURCES += custominput.qrc

OTHER_FILES += doc/src/* \
               doc/images/*