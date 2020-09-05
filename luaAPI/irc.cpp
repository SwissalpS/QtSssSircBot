#include "irc.h"
#include "AppController.h"
#include "CommandEventCodes.h"

#include <QJsonDocument>
#include <QJsonObject>



namespace SwissalpS { namespace QtSssSircBot { namespace luaAPI { namespace IRC {



/// static pointer to shared AppController instance
static AppController *pAC = AppController::pAppController();

// Python versions render different results!
// Note: '**i...**' causes ... to be invisible in docs!
//   '** ...**' causes ... to be displayed as link
//   '**ii...**' works but then it becomes 'iirc.abort'
//   '**as ...**' hides 'irc.abort' too!
//   '**.irc.abort**' becomes '**.irc.abort**'!
//    same with \irc... and _irc...
//   '`irc.abort`' becomes '`irc.abort`'
//   '``irc.abort``' becomes ''!
//   'irc.abort(iNumber)' becomes a link to cppApi/irc.abort(iNumber)
//    same with '<...>'
// that is why we are using "irc.xyz(abc)" for this file

/// Used from Lua by **core.abort**(iNumber) as "irc.abort(iNumber)"
/// \param int iNumber
///    - 0 < iNumber means reload Lua in iNumber milliseconds
///    - 0 >= iNumber means shutdown Lua session. Probably application terminating.
/// \returns void nothing
/// \rst
/// See `core.abort() Lua documentation <../_static/LDoc/modules/core.init.html#core.abort>`_
/// \endrst
static int abortLua(lua_State *L) {
	Q_UNUSED(L)

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString(QChar(CommandEventCodes::Abort))); // 'a'
	aEvent.append(QString::number(luaL_checknumber(L, 1)));

	pAC->onCommandEvent(aEvent);

	return 0;

} // abortLua


/// call from Lua as "irc.add_connection(sJSON)"
/// \param string sJSON the connection configuration as JSON document string.
/// \returns nil or true if sJSON passes JSON syntax test.
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


/// call from Lua as "irc.disconnect(sID)"
///
/// Disconnects socket of connection sID
/// \param string sID the connection ID to disconnect
/// \returns void
static int disconnectSocket(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(CommandEventCodes::Disconnected))); // 'd'
	aEvent.append(QString()); // just to pass initial inspection of argument count

	pAC->onCommandEvent(aEvent);

	return 0;

} // disconnectSocket


/// Used from Lua by **core.quit**() as "irc.exit(iCode)"
///
/// Terminate application with exit code iCode
/// \param int iCode the exit code
/// \returns void
/// \rst
/// See `core.quit() Lua documentation <../_static/LDoc/modules/core.init.html#core.quit>`_
/// \endrst
static int exitApp(lua_State *L) {

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString(QChar(CommandEventCodes::Exit))); // 'e'
	aEvent.append(QString::number(luaL_checknumber(L, 1)));

	pAC->onCommandEvent(aEvent);

	return 0;

} // exitApp


/// call from Lua as "irc.connection_ids()"
/// \returns list of all connection ID strings
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


/// Used from Lua by **core.poll_event**() as "irc.poll_event()"
///
/// handled by LuaController::onIRCevent() and **core.poll_event**()
/// \rst
/// See `core.poll_event() Lua documentation <../_static/LDoc/modules/core.init.html#core.poll_event>`_
/// \endrst
/// \returns nil or event list describing next event
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


/// call from Lua as "irc.reconnect(sID)"
///
/// Reconnects socket of sID using cached parameters (disconnects if connected).
/// \param string sID the connection ID to (re)connect
/// \returns void
static int reconnectSocket(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(CommandEventCodes::Connected))); // 'c'
	aEvent.append(QString()); // just to pass initial inspection of argument count

	pAC->onCommandEvent(aEvent);

	return 0;

} // reconnectSocket


/// call from Lua as "irc.reload_connections()"
///
/// Reads connection config file and reconnects all.
/// Either the config set in Settings.ini is used, or, if present, the one
/// passed as --config command line argument.
/// \returns void
static int reloadConnections(lua_State *L) {
	Q_UNUSED(L)

	QStringList aEvent;
	aEvent.append("-"); // connection ID not relevant here
	aEvent.append(QString(QChar(CommandEventCodes::ReloadConnections))); // 'r'

	pAC->onCommandEvent(aEvent);

	return 0;

} // reloadConnections


/// call from Lua as "irc.send_channel_message(sID, sChannel, sMessage)"
/// \param string sID the connection ID to use
/// \param string sChannel the channel to post on
/// \param string sMessage the message to post on sChannel using connection sID
/// \returns void
static int sendChannelMessage(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(CommandEventCodes::ChannelMessage))); // 'C'
	aEvent.append(QString(luaL_checkstring(L, 2))); // channel
	aEvent.append(QString(luaL_checkstring(L, 3))); // message

	pAC->onCommandEvent(aEvent);

	return 0;

} // sendChannelMessage


/// call from Lua as "irc.send_direct_message(sID, sChannel, sMessage)"
/// \param string sID the connection ID to use
/// \param string sNick the nick to post to
/// \param string sMessage the message to post to sNick using connection sID
/// \returns void
static int sendDirectMessage(lua_State *L) {

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(CommandEventCodes::DirectMessage))); // 'D'
	aEvent.append(QString(luaL_checkstring(L, 2))); // toNick
	aEvent.append(QString(luaL_checkstring(L, 3))); // message

	pAC->onCommandEvent(aEvent);

	return 0;

} // sendDirectMessage


/// call from Lua as "irc.send_join(sID, sChannel)"
/// \param string sID the connection ID to use
/// \param string sChannel the channel to join. Depending on server, this
///   can be a comma separated list of channels
/// \returns void
static int sendJoin(lua_State *L) {

	// fetch connection-id-string and channel

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(CommandEventCodes::Joined))); // 'J'
	aEvent.append(QString(luaL_checkstring(L, 2))); // channel

	pAC->onCommandEvent(aEvent);

	return 0;

} // sendJoin


/// call from Lua as "irc.send_line(sID, sLine)"
/// \param string sID the connection ID to use
/// \param string sLine the raw IRC line to send on connection sID
/// \returns void
static int sendLine(lua_State *L) {

	// fetch connection-id-string and message

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(CommandEventCodes::RawOut))); // '>'
	aEvent.append(QString(luaL_checkstring(L, 2))); // raw line

	pAC->onCommandEvent(aEvent);

	return 0;

} // sendLine


/// call from Lua as "irc.send_nick_change_request(sID, sNewNick)"
/// \param string sID the connection ID to use
/// \param string sNewNick the new nick you want bot to be known as
/// \returns void
static int sendNickChangeRequest(lua_State *L) {

	// fetch connection-id-string and nick

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(CommandEventCodes::NickList))); // 'N'
	aEvent.append(QString(luaL_checkstring(L, 2))); // new nick

	pAC->onCommandEvent(aEvent);

	return 0;

} // sendNickChangeRequest


/// call from Lua as "irc.send_part(sID, sChannels, sMessage = '')"
///
/// Leaves channels.
/// \param string sID the connection ID to use
/// \param string sChannels is a comma separated list of channel names to leave
/// \param string sMessage optional message to post in leave-message
/// \returns void
static int sendPart(lua_State *L) {

	// fetch connection-id-string and channels as comma separated list

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(CommandEventCodes::Part))); // '-'
	aEvent.append(QString(luaL_checkstring(L, 2))); // channels
	aEvent.append(QString(luaL_optstring(L, 3, ""))); // optional message

	pAC->onCommandEvent(aEvent);

	return 0;

} // sendPart


/// call from Lua as "irc.send_quit(sID, sMessage = '')"
///
/// Quits a connection. Usually the server will drop the connection after
/// receiving this command.
/// \param string sID the connection ID to use
/// \param string sMessage optional message to post in quit-message
/// \returns void
static int sendQuit(lua_State *L) {

	// fetch connection-id-string and message

	QStringList aEvent;
	aEvent.append(QString(luaL_checkstring(L, 1))); // connection ID
	aEvent.append(QString(QChar(CommandEventCodes::Quit))); // 'Q'
	aEvent.append(QString(luaL_optstring(L, 2, ""))); // optional quit message

	pAC->onCommandEvent(aEvent);

	return 0;

} // sendLine


/// Translation map Lua => C++ for irc library.
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


/// Loads IRC library into Lua environment.
/// Handled by LuaController::initLua() via luaAPI::loadLibs()
int luaopen(lua_State *L) {

	luaL_newlib(L, lib);

	return 1;

} // luaopen



} } } } // namespace SwissalpS::QtSssSircBot::luaAPI::IRC

