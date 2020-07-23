#include <stdbool.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "api.h"

extern int App_Get_EventC(lua_State *L);

static int f_poll_event(lua_State *L) {

	return App_Get_EventC(L);

}


static const luaL_Reg lib[] = {
	{ "poll_event", f_poll_event },
	{ NULL, NULL }
};


int luaopen_IRC(lua_State *L) {
	luaL_newlib(L, lib);
	return 1;
}
