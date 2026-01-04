TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
QT += core

SOURCES += \
        Sources/backend.cpp \
        Sources/main.cpp \
        Sources/sr_lz4.cpp \
        Sources/sr_processor.cpp \
        Sources/sr_tar.cpp

HEADERS += \
    Sources/backend.h \
    Sources/sr_lz4.h \
    Sources/sr_processor.h \
    Sources/sr_tar.h

MOC_DIR = Build/.moc
RCC_DIR = Build/.rcc
OBJECTS_DIR = Build/.obj

