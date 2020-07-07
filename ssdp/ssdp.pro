TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/common/udp_multicast.c \
    src/mw_ssdp_server.c \
    src/udp_broadcast.c \
    src/udp_broadcast.cpp


HEADERS += \
    inc/cJSON.h \
    inc/common/cJSON.h \
    inc/common/udp_multicast.h \
    inc/mv_ssdp_client.h \
    inc/mv_ssdp_server.h \
    inc/server_implementation.h

DISTFILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lcjson
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lcjson
else:unix: LIBS += -L$$PWD/lib/ -lcjson

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
