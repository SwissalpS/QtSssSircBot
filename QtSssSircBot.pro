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

HEADERS += \
	AppController.h \
	AppSettings.h \
	IRCclient.h \
	IRCclientController.h \
	IRCcommand.h \
	foreign/QtIRC/ircerror.h \
	foreign/QtIRC/ircreply.h \
	foreign/QtIRC/ircservermessage.h

SOURCES += main.cpp \
	AppController.cpp \
	AppSettings.cpp \
	IRCclient.cpp \
	IRCclientController.cpp \
	foreign/QtIRC/ircservermessage.cpp
