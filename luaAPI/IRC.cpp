#include "IRC.h"
#include "AppController.h"
#include "IRCeventCodes.h"

#include <QJsonDocument>
#include <QJsonObject>


namespace SwissalpS { namespace QtSssSircBot { namespace luaAPI { namespace IRC {



static int abortLua(lua_State *L) {
	Q_UNUSED(L)

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString(QChar(IRCeventCodes::Abort))); // 'a'
	aEvent.append(QString::number(luaL_checknumber(L, 1)));

	SwissalpS::QtSssSircBot::AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // abortLua


static int addConnection(lua_State *L) {

	const QJsonDocument oJdoc = QJsonDocument::fromJson(luaL_checkstring(L, 1));
	if (!oJdoc.isObject()) {
		lua_pushnil(L);
		lua_pushstring(L, "invalid JSON, not an object");
		return 2;
	}

	const QJsonObject oJo = oJdoc.object();
	AppController::pAppController()->addConnection(oJo);

	lua_pushboolean(L, true);
	return 1;

} // addConnection


static int disconnectSocket(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::Disconnected))); // 'd'
	aEvent.append(QString()); // just to pass initial inspection of argument count

	AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // disconnectSocket


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


static int reconnectSocket(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::Connected))); // 'c'
	aEvent.append(QString()); // just to pass initial inspection of argument count

	AppController::pAppController()->onLuaEvent(aEvent);

	return 0;

} // reconnectSocket


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


static int sendQuit(lua_State *L) {

	// fetch connection-id-string and message

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(IRCeventCodes::Quit))); // 'Q'
	aEvent.append(QString(luaL_checkstring(L, 2))); // quit message

	AppController::pAppController()->onLuaEvent(aEvent);

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
	{ "send_channel_message", sendChannelMessage },
	{ "send_direct_message", sendDirectMessage },
	{ "send_line", sendLine },
	{ "send_quit", sendQuit },
	{ NULL, NULL }
};


int luaopen(lua_State *L) {

	luaL_newlib(L, lib);

	return 1;

} // luaopen



} } } } // namespace SwissalpS::QtSssSircBot::luaAPI::IRC

