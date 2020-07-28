#ifndef SwissalpS_QtSssSircBot_luaAPI_IRC_H
#define SwissalpS_QtSssSircBot_luaAPI_IRC_H
#include "api.h"



namespace SwissalpS { namespace QtSssSircBot { namespace luaAPI { namespace IRC {



//static int abortLua(lua_State *L);
//static int exitApp(lua_State *L);
//static int getConnectionIDs(lua_State *L);
//static int getEvent(lua_State *L);
//static int sendChannelMessage(lua_State *L);
//static int sendDirectMessage(lua_State *L);
//static int sendLine(lua_State *L);

// not an API command, used to load API
int luaopen(lua_State *L);



} } } } // namespace SwissalpS::QtSssSircBot::luaAPI::IRC



#endif // SwissalpS_QtSssSircBot_luaAPI_IRC_H
