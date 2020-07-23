#include "api.h"


int luaopen_system(lua_State *L);
int luaopen_IRC(lua_State *L);


static const luaL_Reg libs[] = {
  { "system", luaopen_system },
  { "IRC", luaopen_IRC },
  { NULL, NULL }
};

void api_load_libs(lua_State *L) {
  for (int i = 0; libs[i].name; i++) {
	luaL_requiref(L, libs[i].name, libs[i].func, 1);
  }
}

