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



class AppController : public QObject {

	Q_OBJECT

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

private:

protected:
	AppSettings *pAS;
	IRCeventPool *pEP;
	FileCommandInterface *pFCI;
	LuaController *pLC;
	QHash<QString, IRCclientController *> hConnections;
	void connectErrorMessages();
	void initConnections();
	void initFileCommandInterface();
	void initLuaController();
	void initSettings();
	void writePID();

protected slots:

public:
	virtual ~AppController();
	// destroy singelton
	static void drop();
	// public access to singelton instance
	static AppController *pAppController();

	inline QStringList getConnectionIDs() { return this->hConnections.keys(); }

	inline IRCeventPool *getIRCeventPool() { return this->pEP; }

	inline LuaController *getLuaController() { return this->pLC; }

public slots:
	virtual void addConnection(const QJsonObject oConfig);
	virtual void addDelayedCallback(const QString sID, const int iDuration);
	virtual void debugMessage(const QString &sMessage);
	virtual void onCommandEvent(const QStringList &aEvent);
	// depricated, use onCommandEvent()
	Q_DECL_DEPRECATED virtual void onLuaEvent(const QStringList &aEvent);
	virtual void quit();
	virtual void reloadConnections();
	virtual void run();

signals:
	void commandEvent(const QStringList &aEvent);
	void done();
	void debugMessageReceived(const QString &sMessage);
	void luaEvent(const QStringList &aEvent);

}; // AppControler



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_APPCONTROLLER_H
