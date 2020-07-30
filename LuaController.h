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
	QStringList aCLIargs;
	void initLua();

public:
	explicit LuaController(QObject *pParent = nullptr);
	virtual ~LuaController();

	int callLua(const QString sCall);

	void init(QStringList aArgs);

	inline lua_State *getLuaState() { return this->pL; }

	int callDelayedCallback(const QString &sID);
	int callLuaWrapped(const QString sCall);

	static QString wrapLuaCall(const QString sCall);

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("LC:" + sMessage); }
	virtual void onIRCevent();
	virtual void reload();
	virtual void shutdown();

}; // LuaController



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_LUACONTROLLER_H
