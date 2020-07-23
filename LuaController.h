#ifndef SwissalpS_QtSssSircBot_LUACONTROLLER_H
#define SwissalpS_QtSssSircBot_LUACONTROLLER_H

#include <QObject>

extern "C" {
#include "foreign/lua52/lua.h"
#include "foreign/lua52/lauxlib.h"
#include "foreign/lua52/lualib.h"
}



namespace SwissalpS { namespace QtSssSircBot {



class LuaController : public QObject {

	Q_OBJECT

private:

protected:
	lua_State *pL;

public:
	explicit LuaController(QObject *pParent = nullptr);
	virtual ~LuaController();

	void init(QStringList aArgs);

	inline lua_State *getLuaState() { return this->pL; }

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("LC:" + sMessage); }

}; // LuaController



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_LUACONTROLLER_H
