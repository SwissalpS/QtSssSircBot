#ifndef SwissalpS_QtSssSircBot_LuaAPI_API_H
#define SwissalpS_QtSssSircBot_LuaAPI_API_H

extern "C" {

#include "foreign/lua52/lua.h"
#include "foreign/lua52/lauxlib.h"
#include "foreign/lua52/lualib.h"

} // extern "C"



namespace SwissalpS { namespace QtSssSircBot { namespace LuaAPI {



/// Loads irc and system libraries into Lua environment.
/// Handled by LuaController::initLua()
void loadLibs(lua_State *L);



} } } // namespace SwissalpS::QtSssSircBot::LuaAPI



#endif // SwissalpS_QtSssSircBot_LuaAPI_API_H
