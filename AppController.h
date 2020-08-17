#ifndef SwissalpS_QtSssSircBot_APPCONTROLLER_H
#define SwissalpS_QtSssSircBot_APPCONTROLLER_H

#include <QHash>
#include <QObject>
#include <QTcpSocket>
#include <QHostInfo>

#include "AppSettings.h"
#include "FileCommandInterface.h"
#include "IRCclientController.h"



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
	FileCommandInterface *pFCI;
	QHash<QString, IRCclientController *> hConnections;
	void connectErrorMessages();
	void initConnections();
	void initFileCommandInterface();
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

public slots:
	virtual void addConnection(const QJsonObject oConfig);
	virtual void debugMessage(const QString &sMessage);
	virtual void onCommandEvent(const QStringList &aEvent);
	virtual void quit();
	virtual void reloadConnections();
	virtual void run();

signals:
	void commandEvent(const QStringList &aEvent);
	void done();
	void debugMessageReceived(const QString &sMessage);

}; // AppControler



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_APPCONTROLLER_H
