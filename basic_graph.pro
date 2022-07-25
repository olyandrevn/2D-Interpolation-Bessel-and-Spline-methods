QMAKE_CXXFLAGS += -Wall -Werror -W
CONFIG += debug
HEADERS       = window.h \
    interpolation.h
SOURCES       = main.cpp \
                interpolation.cpp \
                window.cpp
QT += widgets
