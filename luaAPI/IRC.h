#ifndef SwissalpS_QtSssSircBot_luaAPI_IRC_H
#define SwissalpS_QtSssSircBot_luaAPI_IRC_H

#include "luaAPI/api.h"

#ifdef __cplusplus
#include "AppController.h"
#include "IRCeventPool.h"
#include "IRCeventCodes.h"

static int luaAPI_IRC_abortLua_Cpp(lua_State *L) {
	Q_UNUSED(L)

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString('a')); // IRCeventCodes::Abort

	SwissalpS::QtSssSircBot::AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // App_Abort_luaCpp

static int luaAPI_IRC_exitApp_Cpp(lua_State *L) {

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString('e')); // IRCeventCodes::Exit
	aEvent.append(QString::number(luaL_checknumber(L, 1)));

	SwissalpS::QtSssSircBot::AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // luaAPI_IRC_exitApp_Cpp

static int luaAPI_IRC_getConnectionIDs_Cpp(lua_State *L) {

	QStringList aList = SwissalpS::QtSssSircBot::AppController::pAppController()->getConnectionIDs();

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

} // App_Get_Connection_IDsCpp

static int luaAPI_IRC_getEvent_Cpp(lua_State *L) {

	SwissalpS::QtSssSircBot::IRCeventPool *pEP =
			SwissalpS::QtSssSircBot::AppController::pAppController()->getIRCeventPool();
	//SwissalpS::QtSssSircBot::LuaController *pLC = pAC->getLuaController();

	//lua_State *L = pLC->getLuaState();
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

} // App_Get_Event

static int luaAPI_IRC_sendChannelMessage_Cpp(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString('C')); // IRCeventCodes::ChannelMessage
	aEvent.append(QString(luaL_checkstring(L, 2))); // channel
	aEvent.append(QString(luaL_checkstring(L, 3))); // message

	SwissalpS::QtSssSircBot::AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // App_Send_Channel_MessageCpp

static int luaAPI_IRC_sendDirectMessage_Cpp(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString('D')); // QChar(IRCeventCodes::DirectMessage)
	aEvent.append(QString(luaL_checkstring(L, 2))); // toNick
	aEvent.append(QString(luaL_checkstring(L, 3))); // message

	SwissalpS::QtSssSircBot::AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // App_Send_Direct_MessageCpp

static int luaAPI_IRC_sendLine_Cpp(lua_State *L) {

	// fetch connection-id-string and message

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString('>')); // QChar(IRCeventCodes::RawOut)
	aEvent.append(QString(luaL_checkstring(L, 2))); // raw line

	SwissalpS::QtSssSircBot::AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // App_Send_LineCpp

extern "C" {
int luaAPI_IRC_abortLua_C(lua_State *L) { return luaAPI_IRC_abortLua_Cpp(L); }
int luaAPI_IRC_exitApp_C(lua_State *L) { return luaAPI_IRC_exitApp_Cpp(L); }
int luaAPI_IRC_getConnectionIDs_C(lua_State *L) { return luaAPI_IRC_getConnectionIDs_Cpp(L); }
int luaAPI_IRC_getEvent_C(lua_State *L) { return luaAPI_IRC_getEvent_Cpp(L); }
int luaAPI_IRC_sendChannelMessage_C(lua_State *L) { return luaAPI_IRC_sendChannelMessage_Cpp(L); }
int luaAPI_IRC_sendDirectMessage_C(lua_State *L) { return luaAPI_IRC_sendDirectMessage_Cpp(L); }
int luaAPI_IRC_sendLine_C(lua_State *L) { return luaAPI_IRC_sendLine_Cpp(L); }
} // extern "C"
#endif // ifdef __cplusplus

#endif // SwissalpS_QtSssSircBot_luaAPI_IRC_H
