TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/common/udp_client.c \
    src/common/udp_server.c \
    src/mw_ssdp_client.c \
    src/mw_ssdp_server.c \
    src/test.c \


HEADERS += \
    inc/common/cJSON.h \
    inc/common/udp_client.h \
    inc/common/udp_com.h \
    inc/common/udp_server.h \
    \ \
    inc/mw_ssdp_client.h \
    inc/mw_ssdp_server.h

DISTFILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lcjson
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lcjson
else:unix: LIBS += -L$$PWD/lib/ -lcjson

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
