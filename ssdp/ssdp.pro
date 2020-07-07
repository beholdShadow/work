TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/mw_ssdp_server.c


HEADERS += \
    inc/cJSON.h \
    inc/mv_ssdp_server.h

DISTFILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lcjson
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lcjson
else:unix: LIBS += -L$$PWD/lib/ -lcjson

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
