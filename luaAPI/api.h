#ifndef SwissalpS_QtSssSircBot_API_H
#define SwissalpS_QtSssSircBot_API_H

#ifdef __cplusplus
#include "AppController.h"
#include "IRCeventPool.h"

static int App_Get_EventCpp(lua_State *L) {

	SwissalpS::QtSssSircBot::IRCeventPool *pEP =
			SwissalpS::QtSssSircBot::AppController::pAppController()->getIRCeventPool();
	//SwissalpS::QtSssSircBot::LuaController *pLC = pAC->getLuaController();

	//lua_State *L = pLC->getLuaState();
	QStringList aEvent = pEP->pollEvent();
	if (aEvent.isEmpty()) return 0;

	QByteArray ba;
	const char *pCstring;

	lua_newtable(L);
	for (int i = 0; i < aEvent.count(); ++i) {

		ba = aEvent.at(i).toLocal8Bit();
		pCstring = ba.data();
		lua_pushstring(L, pCstring);
		lua_rawseti(L, -2, i + 1);

	} // loop

	return 1;

} // App_Get_Event
extern "C" {
#endif

#include "foreign/lua52/lua.h"
#include "foreign/lua52/lauxlib.h"
#include "foreign/lua52/lualib.h"

void api_load_libs(lua_State *L);

#ifdef __cplusplus
int App_Get_EventC(lua_State *L) { return App_Get_EventCpp(L); }
}
#endif

#endif // SwissalpS_QtSssSircBot_API_H
