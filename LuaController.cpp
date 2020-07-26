#include "LuaController.h"

#include "luaAPI/api.h"

// based on info from https://wiki.qt.io/Technical_FAQ#How_can_I_convert_a_QString_to_char.2A_and_vice_versa.3F
// does not seem to work like this as function
const char *qstringToConstCharPointer(QString sQS) {

	QByteArray ba = sQS.toLocal8Bit();

	const char *c_string = ba.data();

	return c_string;

} // qstringToConstCharPointer



namespace SwissalpS { namespace QtSssSircBot {



LuaController::LuaController(QObject *pParent) :
	QObject(pParent) {

} // construct


LuaController::~LuaController() {

} // dealloc


void LuaController::init(QStringList aArgs) {

	int iCountArgs = aArgs.count();
	assert(2 <= iCountArgs);

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	api_load_libs(L);

	lua_newtable(L);
	QByteArray ba;
	const char *pCstring;
	for (int i = 0; i < iCountArgs - 1; i++) {
		ba = aArgs.at(i).toLocal8Bit();
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
			"local core\n"
			"xpcall(function()\n"
			"  SCALE = tonumber(os.getenv(\"LITE_SCALE\")) or SCALE\n"
			"  PATHSEP = package.config:sub(1, 1)\n"
			"  EXEDIR = '" + aArgs.last() + "'\n"
			"  package.path = EXEDIR .. 'data/?.lua;' .. package.path\n"
			"  package.path = EXEDIR .. 'data/?/init.lua;' .. package.path\n"
			"  core = require('core')\n"
			"  core.init()\n"
			"  core.run()\n"
			"end, function(err)\n"
			"  print('Error: ' .. tostring(err))\n"
			"  print(debug.traceback(nil, 2))\n"
			"  if core and core.on_error then\n"
			"    pcall(core.on_error, err)\n"
			"  end\n"
			"  os.exit(1)\n"
			"end)";
/*
	sBootstrap = "xpcall(function()\n"
				 "  print(ARGS[1])\n"
				 "  error('ouchi')\n"
				 "end, function(err)\n"
				 "  print('???? ' .. err)\n"
				 "end)";
	*/
this->onDebugMessage("about to run lua..");

	ba = sBootstrap.toLocal8Bit();
	pCstring = ba.data(); //qstringToConstCharPointer(sBootstrap);
	int iRes = luaL_dostring(L, pCstring);
this->debugMessage(QString::number(iRes) + " ..about to close lua");
	lua_close(L);
	this->onDebugMessage("closed lua.");
} // init



}	} // namespace SwissalpS::QtSssSircBot
