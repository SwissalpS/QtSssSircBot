﻿#include "IRC.h"
#include "AppController.h"
#include "IRCeventCodes.h"

#include <QJsonDocument>
#include <QJsonObject>


namespace SwissalpS { namespace QtSssSircBot { namespace luaAPI { namespace IRC {



static AppController *pAC = AppController::pAppController();


static int abortLua(lua_State *L) {
	Q_UNUSED(L)

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString(QChar(IRCeventCodes::Abort))); // 'a'
	aEvent.append(QString::number(luaL_checknumber(L, 1)));

	pAC->onLuaEvent(aEvent);

	return 0;

} // abortLua


static int addConnection(lua_State *L) {

	const QJsonDocument oJdoc = QJsonDocument::fromJson(luaL_checkstring(L, 1));
	if (!oJdoc.isObject()) {
		lua_pushnil(L);
		lua_pushstring(L, "invalid JSON, not an object");
		return 2;
	}

	pAC->addConnection(oJdoc.object());

	lua_pushboolean(L, true);
	return 1;

} // addConnection


static int disconnectSocket(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::Disconnected))); // 'd'
	aEvent.append(QString()); // just to pass initial inspection of argument count

	pAC->onLuaEvent(aEvent);

	return 0;

} // disconnectSocket


static int exitApp(lua_State *L) {

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString(QChar(IRCeventCodes::Exit))); // 'e'
	aEvent.append(QString::number(luaL_checknumber(L, 1)));

	pAC->onLuaEvent(aEvent);

	return 0;

} // exitApp


static int getConnectionIDs(lua_State *L) {

	QStringList aList = pAC->getConnectionIDs();

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

	IRCeventPool *pEP = pAC->getIRCeventPool();

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


static int reconnectSocket(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::Connected))); // 'c'
	aEvent.append(QString()); // just to pass initial inspection of argument count

	pAC->onLuaEvent(aEvent);

	return 0;

} // reconnectSocket


static int reloadConnections(lua_State *L) {
	Q_UNUSED(L)

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString(QChar(IRCeventCodes::ReloadConnections))); // 'r'

	pAC->onLuaEvent(aEvent);

	return 0;

} // reloadConnections


static int sendChannelMessage(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::ChannelMessage))); // 'C'
	aEvent.append(QString(luaL_checkstring(L, 2))); // channel
	aEvent.append(QString(luaL_checkstring(L, 3))); // message

	pAC->onLuaEvent(aEvent);

	return 0;

} // sendChannelMessage


static int sendDirectMessage(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::DirectMessage))); // 'D'
	aEvent.append(QString(luaL_checkstring(L, 2))); // toNick
	aEvent.append(QString(luaL_checkstring(L, 3))); // message

	pAC->onLuaEvent(aEvent);

	return 0;

} // sendDirectMessage


static int sendJoin(lua_State *L) {

	// fetch connection-id-string and channel

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::Joined))); // 'J'
	aEvent.append(QString(luaL_checkstring(L, 2))); // channel

	pAC->onLuaEvent(aEvent);

	return 0;

} // sendJoin


static int sendLine(lua_State *L) {

	// fetch connection-id-string and message

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::RawOut))); // '>'
	aEvent.append(QString(luaL_checkstring(L, 2))); // raw line

	pAC->onLuaEvent(aEvent);

	return 0;

} // sendLine


static int sendNickChangeRequest(lua_State *L) {

	// fetch connection-id-string and nick

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::NickList))); // 'N'
	aEvent.append(QString(luaL_checkstring(L, 2))); // new nick

	pAC->onLuaEvent(aEvent);

	return 0;

} // sendNickChangeRequest


static int sendPart(lua_State *L) {

	// fetch connection-id-string and channels as comma separated list

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::Part))); // '-'
	aEvent.append(QString(luaL_checkstring(L, 2))); // channels
	aEvent.append(QString(luaL_optstring(L, 3, ""))); // optional message

	pAC->onLuaEvent(aEvent);

	return 0;

} // sendPart


static int sendQuit(lua_State *L) {

	// fetch connection-id-string and message

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::Quit))); // 'Q'
	aEvent.append(QString(luaL_checkstring(L, 2))); // quit message

	pAC->onLuaEvent(aEvent);

	return 0;

} // sendLine


static const luaL_Reg lib[] = {
	{ "abort", abortLua },
	{ "add_connection", addConnection },
	{ "connection_ids", getConnectionIDs },
	{ "disconnect", disconnectSocket },
	{ "exit", exitApp },
	{ "poll_event", getEvent },
	{ "reconnect", reconnectSocket },
	{ "reload_connections", reloadConnections },
	{ "send_channel_message", sendChannelMessage },
	{ "send_direct_message", sendDirectMessage },
	{ "send_join", sendJoin },
	{ "send_line", sendLine },
	{ "send_nick_change_request", sendNickChangeRequest },
	{ "send_part", sendPart },
	{ "send_quit", sendQuit },
	{ NULL, NULL }
};


int luaopen(lua_State *L) {

	luaL_newlib(L, lib);

	return 1;

} // luaopen



} } } } // namespace SwissalpS::QtSssSircBot::luaAPI::IRC

