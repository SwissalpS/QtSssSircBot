#include <stdbool.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "api.h"
#include "IRC.h"

extern int luaAPI_IRC_abortLua_C(lua_State *L);
extern int luaAPI_IRC_exitApp_C(lua_State *L);
extern int luaAPI_IRC_getConnectionIDs_C(lua_State *L);
extern int luaAPI_IRC_getEvent_C(lua_State *L);
extern int luaAPI_IRC_sendChannelMessage_C(lua_State *L);
extern int luaAPI_IRC_sendDirectMessage_C(lua_State *L);
extern int luaAPI_IRC_sendLine_C(lua_State *L);

static int f_get_connection_ids(lua_State *L) {

	return luaAPI_IRC_getConnectionIDs_C(L);

}

static int f_poll_event(lua_State *L) {

	return luaAPI_IRC_getEvent_C(L);

}

static int f_send_channel_message(lua_State *L) {

	return luaAPI_IRC_sendChannelMessage_C(L);

}

static int f_send_direct_message(lua_State *L) {

	return luaAPI_IRC_sendDirectMessage_C(L);

}

static int f_send_line(lua_State *L) {

	return luaAPI_IRC_sendLine_C(L);

}


static const luaL_Reg lib[] = {
	{ "abort", luaAPI_IRC_abortLua_C },
	{ "connection_ids", luaAPI_IRC_getConnectionIDs_C },
	{ "exit", luaAPI_IRC_exitApp_C },
	{ "poll_event", luaAPI_IRC_getEvent_C },
	{ "send_channel_message", luaAPI_IRC_sendChannelMessage_C },
	{ "send_direct_message", luaAPI_IRC_sendDirectMessage_C },
	{ "send_line", luaAPI_IRC_sendLine_C },
	{ NULL, NULL }
};


int luaopen_IRC(lua_State *L) {
	luaL_newlib(L, lib);
	return 1;
}
