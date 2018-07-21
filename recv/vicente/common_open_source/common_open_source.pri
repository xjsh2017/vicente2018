INCLUDEPATH += $$PWD


contains(DEFINES, XJSTATUSSTAT_COMM_OPEN_PRI){
    message("THIS IS XJSTATUSSTAT COMMON OPEN PRI")
# Input
    HEADERS += \
               $$PWD/cJSON/cJSON.h

    SOURCES += $$PWD/cJSON/cJSON.c
}

contains(DEFINES, XJFAULTREPORTTOXML_COMM_OPEN_PRI){
    message("THIS IS XJFAULTREPORTTOXML COMMON OPEN PRI")
# Input
    HEADERS += \
               $$PWD/tinyxml.h \
               $$PWD/tinyxml2.h \
               $$PWD/tinystr.h

    SOURCES += \
               $$PWD/tinyxml.cpp \
               $$PWD/tinyxml2.cpp \
               $$PWD/tinyxmlerror.cpp \
               $$PWD/tinyxmlparser.cpp \
               $$PWD/tinystr.cpp
}

contains(DEFINES, XJAUTOCHECK_COMM_OPEN_PRI){
    message("THIS IS XJAUTOCHECK COMMON OPEN PRI")
# Input
    HEADERS += \
               $$PWD/tinyxml.h \
               $$PWD/tinyxml2.h \
               $$PWD/tinystr.h

    SOURCES += \
               $$PWD/tinyxml.cpp \
               $$PWD/tinyxml2.cpp \
               $$PWD/tinyxmlerror.cpp \
               $$PWD/tinyxmlparser.cpp \
               $$PWD/tinystr.cpp
}
