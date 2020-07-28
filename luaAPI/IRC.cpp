﻿#include "IRC.h"
#include "AppController.h"
#include "IRCeventCodes.h"



namespace SwissalpS { namespace QtSssSircBot { namespace luaAPI { namespace IRC {



static int abortLua(lua_State *L) {
	Q_UNUSED(L)

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString(QChar(IRCeventCodes::Abort))); // 'a'

	SwissalpS::QtSssSircBot::AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // abortLua


static int exitApp(lua_State *L) {

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString(QChar(IRCeventCodes::Exit))); // 'e'
	aEvent.append(QString::number(luaL_checknumber(L, 1)));

	AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // exitApp


static int getConnectionIDs(lua_State *L) {

	QStringList aList = AppController::pAppController()->getConnectionIDs();

	QByteArray ba;
	const char *pCstring;

	lua_newtable(L);
	for (int i = 0; i < aList.count(); ++i) {

		ba = aList.at(i).toLocal8Bit();
		pCstring = ba.data();
		lua_pushstring(L, pCstring);
		lua_rawseti(L, -2, i + 1);

	} // loop

	return 1;

} // getConnectionIDs


static int getEvent(lua_State *L) {

	IRCeventPool *pEP = AppController::pAppController()->getIRCeventPool();

	QStringList aEvent = pEP->pollEvent();
	if (aEvent.isEmpty()) {
		lua_pushnil(L);
		return 1;
	}

	QByteArray ba;
	const char *pCstring;

	lua_newtable(L);
	for (int i = 0; i < aEvent.count(); ++i) {

		ba = aEvent.at(i).toLocal8Bit();
		pCstring = ba.data();
		lua_pushstring(L, pCstring);
		lua_rawseti(L, -2, i + 1);

	} // loop

	return 1;

} // getEvent


static int sendChannelMessage(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::ChannelMessage))); // 'C'
	aEvent.append(QString(luaL_checkstring(L, 2))); // channel
	aEvent.append(QString(luaL_checkstring(L, 3))); // message

	AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // sendChannelMessage


static int sendDirectMessage(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::DirectMessage))); // 'D'
	aEvent.append(QString(luaL_checkstring(L, 2))); // toNick
	aEvent.append(QString(luaL_checkstring(L, 3))); // message

	AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // sendDirectMessage


static int sendLine(lua_State *L) {

	// fetch connection-id-string and message

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::RawOut))); // '>'
	aEvent.append(QString(luaL_checkstring(L, 2))); // raw line

	AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // sendLine


static const luaL_Reg lib[] = {
	{ "abort", abortLua },
	{ "connection_ids", getConnectionIDs },
	{ "exit", exitApp },
	{ "poll_event", getEvent },
	{ "send_channel_message", sendChannelMessage },
	{ "send_direct_message", sendDirectMessage },
	{ "send_line", sendLine },
	{ NULL, NULL }
};


int luaopen(lua_State *L) {

	luaL_newlib(L, lib);

	return 1;

} // luaopen



} } } } // namespace SwissalpS::QtSssSircBot::luaAPI::IRC
