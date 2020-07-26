#ifndef SwissalpS_QtSssSircBot_luaAPI_API_H
#define SwissalpS_QtSssSircBot_luaAPI_API_H
#ifdef __cplusplus
extern "C" {
#endif

#include "foreign/lua52/lua.h"
#include "foreign/lua52/lauxlib.h"
#include "foreign/lua52/lualib.h"

void api_load_libs(lua_State *L);

#ifdef __cplusplus
}
#endif

#endif // SwissalpS_QtSssSircBot_luaAPI_API_H
