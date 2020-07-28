#include "system.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <QString>
#include <QStringList>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


namespace SwissalpS { namespace QtSssSircBot { namespace luaAPI { namespace System {


// all the functions starting with 'f_' are taken from github.com/rxi/lite
// only a few changes were needed to make them compile as Cpp code.

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

} // f_list_dir


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


static int initRand(lua_State *L) {
	Q_UNUSED(L)

	time_t t;
	srand((unsigned) time(&t));

	return 0;

} // initRand


static int getRand(lua_State *L) {

	int n = rand();
	lua_pushnumber(L, n);

	return 1;

} // getRand


static int f_exec(lua_State *L) {

	size_t len;
	const char *cmd = luaL_checklstring(L, 1, &len);
	char *buf = (char *)malloc(len + 32);
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


static int splitString(lua_State *L) {

	const char *pIn = luaL_checkstring(L, 1);
	const char *pDelim = luaL_checkstring(L, 2);
	const QString sIn(pIn);
	const QString sDelim(pDelim);
	const QStringList aOut = sIn.split(sDelim, QString::SkipEmptyParts, Qt::CaseSensitive);

	lua_newtable(L);

	// no parts -> return input string in table
	if (0 == aOut.count()) {

		lua_pushstring(L, pIn);
		lua_rawseti(L, -2, 1);

		return 1;

	} // if no parts

	QByteArray ba;
	const char *pCstring;
	for (int i = 0; i < aOut.count(); ++i) {

		ba = aOut.at(i).toLocal8Bit();
		pCstring = ba.data();
		lua_pushstring(L, pCstring);
		lua_rawseti(L, -2, i + 1);

	} // loop all parts into table

	return 1;

} // luaAPIcpp_system_splitString


static const luaL_Reg lib[] = {
	{ "absolute_path", f_absolute_path },
	{ "chdir", f_chdir },
	{ "exec", f_exec },
	{ "get_file_info", f_get_file_info },
	{ "get_rand", getRand },
	{ "init_rand", initRand },
	{ "list_dir", f_list_dir },
	{ "split_string", splitString },
	{ NULL, NULL }
};


int luaopen(lua_State *L) {

	luaL_newlib(L, lib);

	return 1;

} // luaopen



} } } } // namespace SwissalpS::QtSssSircBot::luaAPI::system
