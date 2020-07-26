#ifndef SwissalpS_QtSssSircBot_luaAPI_SYSTEM_H
#define SwissalpS_QtSssSircBot_luaAPI_SYSTEM_H

#include "api.h"

#ifdef __cplusplus
#include <QString>
#include <QStringList>
static int luaAPI_system_splitString_Cpp(lua_State *L) {

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

} // luaAPI_system_splitString_Cpp


extern "C" {
int luaAPI_system_splitString_C(lua_State *L) { return luaAPI_system_splitString_Cpp(L); }
} // extern "C"
#endif // ifdef __cplusplus

#endif // SwissalpS_QtSssSircBot_luaAPI_SYSTEM_H
