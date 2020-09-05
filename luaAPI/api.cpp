//#include "api.h"
#include "IRC.h"
#include "system.h"



namespace SwissalpS { namespace QtSssSircBot { namespace luaAPI {



static const luaL_Reg libs[] = {
	{ "system", System::luaopen },
	{ "irc", IRC::luaopen },
	{ NULL, NULL }
};


void loadLibs(lua_State *L) {

	for (int i = 0; libs[i].name; i++) {

		luaL_requiref(L, libs[i].name, libs[i].func, 1);

	}

} // apiLoadLibs



} } } // namespace SwissalpS::QtSssSircBot::luaAPI
