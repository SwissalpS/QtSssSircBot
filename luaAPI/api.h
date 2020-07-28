#ifndef SwissalpS_QtSssSircBot_luaAPI_API_H
#define SwissalpS_QtSssSircBot_luaAPI_API_H

extern "C" {

#include "foreign/lua52/lua.h"
#include "foreign/lua52/lauxlib.h"
#include "foreign/lua52/lualib.h"

} // extern "C"



namespace SwissalpS { namespace QtSssSircBot { namespace luaAPI {



void loadLibs(lua_State *L);



} } } // namespace SwissalpS::QtSssSircBot::luaAPI



#endif // SwissalpS_QtSssSircBot_luaAPI_API_H
