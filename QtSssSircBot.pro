## only if you want GUI
#DEFINES += SssS_USE_GUI

QT *= core network
contains(DEFINES, SssS_USE_GUI) {
	QT *= gui
	QT *= widgets
#	QT *= multimediawidgets
} else {
	QT -= gui
	CONFIG *= console
}

CONFIG *= c++11
CONFIG -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

## AppVersion format major_minor
VERSION = 0.0.1
## App name short (no spaces, :, ;, /, \ etc.)
## next portion will use this to make a longer one
SssS_APP_NAME = QtSssSircBot
## load OS detection (after setting VERSION and SssS_APP_NAME)
## this should set some vars and DEFINES to help switch
## main variable is SssS_APP_NAME_FULL
exists("$$_PRO_FILE_PWD_/SssS_OS.pri"):include($$_PRO_FILE_PWD_/SssS_OS.pri) {
#	message($$SssS_APP_NAME_FULL)
} else {
	error("Could not load OS detection portion SssS_OS.pri")
}
TARGET = $${SssS_APP_NAME_FULL}

# add deploy target for raspberry-pi
CONFIG(SssS_RasPi) {
#message("RasPi")
	target.files = $${SssS_APP_NAME_FULL}
	target.path = /home/pi/bin
	INSTALLS += target
}

DEFINES += LUA_USE_POSIX

HEADERS += \
	AppController.h \
	AppSettings.h \
	IRCclient.h \
	IRCclientController.h \
	IRCcommand.h \
	IRCeventCodes.h \
	IRCeventPool.h \
	LuaController.h \
	foreign/QtIRC/ircerror.h \
	foreign/QtIRC/ircreply.h \
	foreign/QtIRC/ircservermessage.h \
	foreign/lua52/lapi.h \
	foreign/lua52/lauxlib.h \
	foreign/lua52/lcode.h \
	foreign/lua52/lctype.h \
	foreign/lua52/ldebug.h \
	foreign/lua52/ldo.h \
	foreign/lua52/lfunc.h \
	foreign/lua52/lgc.h \
	foreign/lua52/llex.h \
	foreign/lua52/llimits.h \
	foreign/lua52/lmem.h \
	foreign/lua52/lobject.h \
	foreign/lua52/lopcodes.h \
	foreign/lua52/lparser.h \
	foreign/lua52/lstate.h \
	foreign/lua52/lstring.h \
	foreign/lua52/ltable.h \
	foreign/lua52/ltm.h \
	foreign/lua52/lua.c_ \
	foreign/lua52/lua.h \
	foreign/lua52/lua.hpp \
	foreign/lua52/luac.c_ \
	foreign/lua52/luaconf.h \
	foreign/lua52/lualib.h \
	foreign/lua52/lundump.h \
	foreign/lua52/lvm.h \
	foreign/lua52/lzio.h \
	luaAPI/IRC.h \
	luaAPI/api.h \
	luaAPI/system.h

SOURCES += main.cpp \
	AppController.cpp \
	AppSettings.cpp \
	IRCclient.cpp \
	IRCclientController.cpp \
	IRCeventPool.cpp \
	LuaController.cpp \
	foreign/QtIRC/ircservermessage.cpp \
	foreign/lua52/lapi.c \
	foreign/lua52/lauxlib.c \
	foreign/lua52/lbaselib.c \
	foreign/lua52/lbitlib.c \
	foreign/lua52/lcode.c \
	foreign/lua52/lcorolib.c \
	foreign/lua52/lctype.c \
	foreign/lua52/ldblib.c \
	foreign/lua52/ldebug.c \
	foreign/lua52/ldo.c \
	foreign/lua52/ldump.c \
	foreign/lua52/lfunc.c \
	foreign/lua52/lgc.c \
	foreign/lua52/linit.c \
	foreign/lua52/liolib.c \
	foreign/lua52/llex.c \
	foreign/lua52/lmathlib.c \
	foreign/lua52/lmem.c \
	foreign/lua52/loadlib.c \
	foreign/lua52/lobject.c \
	foreign/lua52/lopcodes.c \
	foreign/lua52/loslib.c \
	foreign/lua52/lparser.c \
	foreign/lua52/lstate.c \
	foreign/lua52/lstring.c \
	foreign/lua52/lstrlib.c \
	foreign/lua52/ltable.c \
	foreign/lua52/ltablib.c \
	foreign/lua52/ltm.c \
	foreign/lua52/lundump.c \
	foreign/lua52/lvm.c \
	foreign/lua52/lzio.c \
	luaAPI/IRC.cpp \
	luaAPI/api.cpp \
	luaAPI/system.cpp

RESOURCES += \
	QtSssSircBot.qrc
