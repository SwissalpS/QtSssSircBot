#include "LuaController.h"

#include "luaAPI/IRC.h"
#include "luaAPI/system.h"

// based on info from https://wiki.qt.io/Technical_FAQ#How_can_I_convert_a_QString_to_char.2A_and_vice_versa.3F
// does not seem to work like this as function
const char *qstringToConstCharPointer(QString sQS) {

	QByteArray ba = sQS.toLocal8Bit();

	const char *c_string = ba.data();

	return c_string;

} // qstringToConstCharPointer



namespace SwissalpS { namespace QtSssSircBot {



LuaController::LuaController(QObject *pParent) :
	QObject(pParent),
	pL(nullptr) {

	this->aCLIargs.clear();

} // construct


LuaController::~LuaController() {

	if (this->pL) {

		lua_close(this->pL);
		this->pL = nullptr;

	}

} // dealloc


int LuaController::callDelayedCallback(const QString &sID) {

	return this->callLua("core.call_delayed_callback('" + sID + "')");

} // callDelayedCallback


// returns 0 on success
int LuaController::callLua(const QString sCall) {

	if (!this->pL) return -1;

	QByteArray ba = sCall.toLocal8Bit();
	const char *pCstring = ba.data();

	return luaL_dostring(this->pL, pCstring);

} // callLua


// returns 0 on success
int LuaController::callLuaWrapped(const QString sCall) {

	return this->callLua(wrapLuaCall(sCall));

} // callLuaWrapped


void LuaController::init(QStringList aArgs) {

	// need at least 2 args!

	this->aCLIargs.clear();
	this->aCLIargs.append(aArgs);

	this->reload();

} // init


void LuaController::initLua() {

	if (this->pL) return;

	int iCountArgs = this->aCLIargs.count();
//	assert(2 <= iCountArgs);

	// init lua state
	this->pL = luaL_newstate();
	// for convenience we use a local pointer
	lua_State *L = this->pL;

	// load libraries
	luaL_openlibs(L);
	luaAPI::loadLibs(L);

	// pass cli-arguments, possibly some more,
	// to lua global space
	lua_newtable(L);
	QByteArray ba;
	const char *pCstring;
	for (int i = 0; i < iCountArgs - 1; i++) {
		ba = this->aCLIargs.at(i).toLocal8Bit();
		pCstring = ba.data();
		lua_pushstring(L, pCstring);
		lua_rawseti(L, -2, i + 1);
	}
	lua_setglobal(L, "ARGS");

	lua_pushstring(L, SssS_APP_VERSION_DOTTED);
	lua_setglobal(L, "VERSION");

	lua_pushstring(L, SssS_APP_NAME_FULL); //SDL_GetPlatform());
	lua_setglobal(L, "PLATFORM");

	lua_pushstring(L, SssS_APP_NAME);
	lua_setglobal(L, "EXEFILE");

	QString sBootstrap =
			"core = {}\n" + wrapLuaCall(
				// modify print function for now, to help detect where debug
				// messages are comming from
			"  local print_orig = print\n"
			"  print = function(...) print_orig('L:', ...) end\n"
				// detect path separator
			"  PATHSEP = package.config:sub(1, 1)\n"
				// last argument is not actually a cli-argument
			"  EXEDIR = '" + this->aCLIargs.last() + "'\n"
				// prepare auto-load mechanism
			"  package.path = EXEDIR .. 'lua/?.lua;' .. package.path\n"
			"  package.path = EXEDIR .. 'lua/?/init.lua;' .. package.path\n"
				// load core/init.lua
			"  core = require('core')\n"
				// init lua side, load plugins etc.
			"  core.init()\n"
//			"  core.run()\n"
				);

	int iRes = this->callLua(sBootstrap);
	this->onDebugMessage("lua init: " + QString::number(iRes));

} // initLua


void LuaController::onIRCevent() {

	if (!this->pL) return;

	this->callLuaWrapped("core.poll_event()");

} // onIRCevent


void LuaController::reload() {

	this->shutdown();

	this->initLua();

} // reload


void LuaController::shutdown() {

	if (this->pL) {

		lua_close(this->pL);
		this->pL = nullptr;

	}

} // shutdown


QString LuaController::wrapLuaCall(const QString sCall) {

	QString sOut =
			"xpcall(function()\n" +	sCall + "\n"
			"end, function(err)\n"
			"  print('Error: ' .. tostring(err))\n"
			"  print(debug.traceback(nil, 2))\n"
			"  if core and core.on_error then\n"
			"    pcall(core.on_error, err)\n"
			"  end\n"
			"end)";

	return sOut;

} // wrapLuaCall



}	} // namespace SwissalpS::QtSssSircBot
