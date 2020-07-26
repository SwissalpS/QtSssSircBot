#include <stdbool.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "api.h"


static int f_chdir(lua_State *L) {

	const char *path = luaL_checkstring(L, 1);
	int err = chdir(path);
	if (err) { luaL_error(L, "chdir() failed"); }

	return 0;

} // f_chdir


static int f_list_dir(lua_State *L) {
	const char *path = luaL_checkstring(L, 1);

	DIR *dir = opendir(path);
	if (!dir) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		return 2;
	}

	lua_newtable(L);
	int i = 1;
	struct dirent *entry;
	while ( (entry = readdir(dir)) ) {
		if (strcmp(entry->d_name, "." ) == 0) { continue; }
		if (strcmp(entry->d_name, "..") == 0) { continue; }
		lua_pushstring(L, entry->d_name);
		lua_rawseti(L, -2, i);
		i++;
	}

	closedir(dir);
	return 1;
}


#ifdef _WIN32
#include <windows.h>
#define realpath(x, y) _fullpath(y, x, MAX_PATH)
#endif

static int f_absolute_path(lua_State *L) {

	const char *path = luaL_checkstring(L, 1);
	char *res = realpath(path, NULL);
	if (!res) { return 0; }

	lua_pushstring(L, res);

	free(res);

	return 1;

} // f_absolute_path


static int f_get_file_info(lua_State *L) {

	const char *path = luaL_checkstring(L, 1);

	struct stat s;
	int err = stat(path, &s);
	if (err < 0) {
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		return 2;
	}

	lua_newtable(L);
	lua_pushnumber(L, s.st_mtime);
	lua_setfield(L, -2, "modified");

	lua_pushnumber(L, s.st_size);
	lua_setfield(L, -2, "size");

	if (S_ISREG(s.st_mode)) {
		lua_pushstring(L, "file");
	} else if (S_ISDIR(s.st_mode)) {
		lua_pushstring(L, "dir");
	} else {
		lua_pushnil(L);
	}
	lua_setfield(L, -2, "type");

	return 1;

} // f_get_file_info


static int f_exec(lua_State *L) {

	size_t len;
	const char *cmd = luaL_checklstring(L, 1, &len);
	char *buf = malloc(len + 32);
	if (!buf) { luaL_error(L, "buffer allocation failed"); }
#if _WIN32
	sprintf(buf, "cmd /c \"%s\"", cmd);
	WinExec(buf, SW_HIDE);
#else
	sprintf(buf, "%s &", cmd);
	int res = system(buf);
	(void) res;
#endif
	free(buf);

	return 0;

} // f_exec


static const luaL_Reg lib[] = {
	{ "chdir", f_chdir },
	{ NULL, NULL }
};


int luaopen_system(lua_State *L) {
	luaL_newlib(L, lib);
	return 1;
}
