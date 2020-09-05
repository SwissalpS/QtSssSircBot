#ifndef SwissalpS_QtSssSircBot_LUACONTROLLER_H
#define SwissalpS_QtSssSircBot_LUACONTROLLER_H

#include <QObject>

extern "C" {
#include "foreign/lua52/lua.h"
#include "foreign/lua52/lauxlib.h"
#include "foreign/lua52/lualib.h"
}



namespace SwissalpS { namespace QtSssSircBot {



/// \brief Class to manage a Lua environment.
///
/// Use init(QStringList aArgs) to get Lua environment started.
/// Use callLuaWrapped() to execute Lua code in environment
/// \rst
/// `Read more about Lua environment <../_static/LDoc/index.html>`_
/// \endrst
class LuaController : public QObject {

	Q_OBJECT

private:

protected:
	/// pointer to the Lua environment
	lua_State *pL;
	/// list of command line arguments plus paths at end merged in
	QStringList aCLIargs;
	/// initialize Lua State and bootstrap Lua environment
	void initLua();

public:
	/// constructor
	explicit LuaController(QObject *pParent = nullptr);
	virtual ~LuaController();

	/// execute given Lua call in Lua environment.
	/// \see callLuaWrapped()
	int callLua(const QString sCall);

	/// initialize object and start Lua environment
	/// TODO: add more description about aArgs
	void init(QStringList aArgs);

	/// get pointer of Lua Environment
	inline lua_State *getLuaState() { return this->pL; }

	/// calls core.call_delayed_callback() in Lua environment.
	/// This is triggered by a delayed callback timer running out.
	/// \see DelayedCallback
	int callDelayedCallback(const QString &sID);
	/// execute given Lua-call aftre wrapping it into an xpcall(...)
	/// with wrapLuaCall()
	int callLuaWrapped(const QString sCall);

	/// wrap given Lua-call into an xpcall(...)
	static QString wrapLuaCall(const QString sCall);

signals:
	/// general debug messages, mute output or don't connect for release
	void debugMessage(const QString &sMessage) const;

public slots:
	/// signal a message for debugging
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("LC:" + sMessage); }

	/// calls core.poll_event() in Lua environment.
	///
	/// This causes getEvent() to be called on C++ side through IRC Lua-API
	/// All a reaction to incomming messages to an IRCclient.
	virtual void onIRCevent();
	/// shutdown() and initLua()
	virtual void reload();
	/// shutdown Lua environment
	virtual void shutdown();

}; // LuaController



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_LUACONTROLLER_H
