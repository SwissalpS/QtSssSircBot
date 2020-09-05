#ifndef SwissalpS_QtSssSircBot_DELAYEDCALLBACK_H
#define SwissalpS_QtSssSircBot_DELAYEDCALLBACK_H

#include "LuaController.h"

#include <QObject>



namespace SwissalpS { namespace QtSssSircBot {


/// \brief Simple wrapper to QTimer::singleshot().
///
/// Used to give Lua environment a way to execute timed code.
/// Hold on to that pointer for performance reasons.
/// \rst
/// `Read more about ... <../luaAPI/xyz.html>`_
/// \endrst
class DelayedCallback : public QObject {

	Q_OBJECT

private:

protected:
	/// idintifier string
	QString sIdent;
	/// interval to wait
	int iInterval;
	/// pointer to LuaController instance to trigger callbacks with
	LuaController *pLuaController;

public:
	/// constructor.
	/// \param sID identifier which Lua environment will recognize
	/// \param iDuration in how many milliseconds to execute callback
	/// \param pLC pointer to LuaController instance to trigger callbacks with
	/// \param pParent parent QObject
	explicit DelayedCallback(const QString &sID, const int &iDuration,
							 LuaController *pLC, QObject *pParent = nullptr);
	virtual ~DelayedCallback();

	/// start timer
	void start();

signals:

public slots:
	/// called when timer runs out.
	///
	/// - calls LuaController::callDelayedCallback()
	/// - deletes itself
	void onDone();

}; // DelayedCallback



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_DELAYEDCALLBACK_H
