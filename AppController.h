#ifndef SwissalpS_QtSssSircBot_APPCONTROLLER_H
#define SwissalpS_QtSssSircBot_APPCONTROLLER_H

#include <QHash>
#include <QObject>
#include <QTcpSocket>
#include <QHostInfo>

#include "AppSettings.h"
#include "FileCommandInterface.h"
#include "IRCclientController.h"
#include "IRCeventPool.h"
#include "LuaController.h"



namespace SwissalpS { namespace QtSssSircBot {



/// \brief Singleton class to control the application.
///
/// Use pAppController() to get hold of pointer to shared instance.
/// Hold on to that pointer for performance reasons.
///
/// This is the main hub, holding all parts together and orchestrating.
/// Entry point is run()
class AppController : public QObject {

	Q_OBJECT

private:
	static AppController *pSingelton;
	// keep this private as we want only one instance
	// read; http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern
	explicit AppController(QObject *pParent = 0);
	// hide copy constructor
	explicit AppController(const AppController &other);
	// hide assign op
	// we leave just the declarations, so the compiler will warn us
	// if we try to use those two functions by accident
	AppController& operator=(const AppController &other);

protected:
	/// holds pointer to shared QtSssSircBot::AppSettings instance.
	AppSettings *pAS;
	/// holds pointer to IRCeventPool instance
	IRCeventPool *pEP;
	/// holds pointer to backdoor handler
	FileCommandInterface *pFCI;
	/// holds pointer to LuaController instance
	LuaController *pLC;
	/// holds connections by ID
	QHash<QString, IRCclientController *> hConnections;
	/// not currently used. Designed to connect error signals. Makes sense for
	/// some projects to use this.
	void connectErrorMessages();
	/// start connecting
	void initConnections();
	/// initialize and start the backdoor handler
	void initFileCommandInterface();
	/// initialize and start the Lua environment
	void initLuaController();
	/// initialize settings, prepare work directory and parse command line arguments
	void initSettings();
	/// write PID to file
	void writePID();

protected slots:

public:
	virtual ~AppController();
	/// destroy singelton, no need to call this
	static void drop();
	/// public access to singelton instance. Hold on to the pointer.
	static AppController *pAppController();

	/// get a list with all the connection IDs
	inline QStringList getConnectionIDs() { return this->hConnections.keys(); }

	/// get pointer to event pool
	inline IRCeventPool *getIRCeventPool() { return this->pEP; }

	/// get pointer to LuaController
	inline LuaController *getLuaController() { return this->pLC; }

public slots:
	/// add a connection and start it
	virtual void addConnection(const QJsonObject oConfig);
	/// add a delayed call back
	virtual void addDelayedCallback(const QString sID, const int iDuration);
	/// signal a message for debugging TODO: rename to onDebugMessage for consistancy.
	///
	/// This is the end point of all messages, after this debugMessageReceived()
	/// is emitted and whoever is listening to that can log/display the
	/// messages sent by any class supporting this system.
	virtual void debugMessage(const QString &sMessage);
	/// interfaces like FileCommandInterface send signals here.
	virtual void onCommandEvent(const QStringList &aEvent);
	/// depricated, use onCommandEvent()
	Q_DECL_DEPRECATED virtual void onLuaEvent(const QStringList &aEvent);
	/// terminate the application
	virtual void quit();
	/// re-read connection data from config and reconnect
	virtual void reloadConnections();
	/// main entry point called from main() via single-shot timer
	virtual void run();

signals:
	/// emitted from onCommandEvent() when it didn't handle the event
	void commandEvent(const QStringList &aEvent);
	/// emitted to signal QCoreApplication/QApplication to exit.
	/// there should be no need to emit this signal. Use quit().
	void done();
	/// emitted when a message was dropped in debugMessage()
	void debugMessageReceived(const QString &sMessage);
	void luaEvent(const QStringList &aEvent);

}; // AppControler



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_APPCONTROLLER_H

