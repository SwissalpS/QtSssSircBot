#include "AppController.h"
#include "CommandEventCodes.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QHostInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMutex>
#include <QProcess>
#include <QStandardPaths>
#include <QTimer>
#include <iostream>



namespace SwissalpS { namespace QtSssSircBot {


AppController *AppController::pSingelton = 0;

AppController::AppController(QObject *pParent) :
	QObject(pParent),
	pAS(nullptr),
	pFCI(nullptr) {

	this->hConnections.clear();

} // construct


AppController::~AppController() {

	// TODO: quit all connections and destroy objects

} // dealloc


void AppController::drop() {

	static QMutex oMutex;

	oMutex.lock();

	delete pSingelton;
	pSingelton = 0;

	oMutex.unlock();

} // drop


AppController *AppController::pAppController() {

	///
	/// \brief oMutex
	///
	static QMutex oMutex;

	///////////////////////////////
	/* NOTE from: http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern
	 * Be aware that you should check for a std::bad_alloc exception
	 * to unlock the mutex in case new fails. We are also using
	 * double-checked locking as locking and unlocking a mutex
	 * is expensive and would significantly slow down a call to
	 *  Singleton::instance().
	 */
	if (!AppController::pSingelton) {

		oMutex.lock();

		if (!pSingelton) {

			pSingelton = new AppController();

		} // if first call

		oMutex.unlock();

	} // if first call

	return pSingelton;

} // singelton access


void AppController::addConnection(const QJsonObject oConfig) {

	IRCclientController *pController = new IRCclientController(oConfig, this);
	const QString sID = pController->getConnectionID();

	if (this->hConnections.contains(sID)) {

		this->debugMessage("AC:OO:Duplicate connection ID: <" + sID +
							 "> found. Skipping. Make sure all your "
							 "connections have unique identifiers.");

		pController->deleteLater();

	} else {

		this->hConnections.insert(sID, pController);

		connect(pController, SIGNAL(debugMessage(QString)),
				this, SLOT(debugMessage(QString)));

//			connect(pController, SIGNAL(abort(qint16)),
//					this, SLOT(onAbort(qint16)));

//		connect(pController, SIGNAL(newEvent(QStringList)),
//				this->pEP, SLOT(onEvent(QStringList)));

		// it's up to each controller to drop events not targeted at them
		connect(this, SIGNAL(commandEvent(QStringList)),
				pController, SLOT(onCommandEvent(QStringList)));

		pController->start();

	} // if valid connection-ID or not

} // addConnection


void AppController::connectErrorMessages() {

	//AppSettings *pAS = AppSettings::pAppSettings();

} // connectErrorMessages


void AppController::debugMessage(const QString &sMessage) {

	std::cout << sMessage.toStdString();
	std::cout << std::endl;

	Q_EMIT debugMessageReceived(sMessage);

} // debugMessage


void AppController::initConnections() {

	const QJsonArray oConfigs = this->pAS->getConfigs();

	QJsonObject oConfig;

	for (int i = 0; i < oConfigs.count(); ++i) {

		oConfig = oConfigs.at(i).toObject();

		this->addConnection(oConfig);

	} // loop all connections

} // initConnections


void AppController::initFileCommandInterface() {

	if (nullptr != this->pFCI) return;

	int iInterval = this->pAS->get(AppSettings::sSettingBackdoorIntervalMS).toInt();
	this->pFCI = new FileCommandInterface(this->pAS->getPathBackdoor(),
										  iInterval, this);

	connect(this->pFCI, SIGNAL(debugMessage(QString)),
			this, SLOT(debugMessage(QString)));

	connect(this->pFCI, SIGNAL(newEvent(QStringList)),
			this, SLOT(onCommandEvent(QStringList)));

	this->pFCI->start();

} // initFileCommandInterface


void AppController::initSettings() {

	this->pAS = AppSettings::pAppSettings();

	connect(pAS, SIGNAL(debugMessage(QString)),
			this, SLOT(debugMessage(QString)));

	this->pAS->init();

} // initSettings


void AppController::onCommandEvent(const QStringList &aEvent) {

	int iEvent = aEvent.count();
	quint8 ubEvent = aEvent.at(1).at(0).unicode();
	if ((3 <= iEvent) && (CommandEventCodes::Abort == ubEvent)) {

// not used in this branch
//		int iRes = aEvent.at(2).toInt();
//		if (0 < iRes) {
//			QTimer::singleShot(iRes, this->pLC, SLOT(reload()));
//		} else {
//			QTimer::singleShot(0, this->pLC, SLOT(shutdown()));
//		}

	} else if (CommandEventCodes::Exit == ubEvent) {

		QTimer::singleShot(1000, this, SLOT(quit()));

	} else if (CommandEventCodes::ReloadConnections == ubEvent) {

		QTimer::singleShot(0, this, SLOT(reloadConnections()));

	} else Q_EMIT this->commandEvent(aEvent);

} // onCommandEvent


void AppController::quit() {

	AppSettings::pAppSettings()->getSettings()->sync();

	QStringList aKeys = this->hConnections.keys();
	IRCclientController *pCC;
	for (int i = 0; i < aKeys.count(); ++i) {

		pCC = this->hConnections.value(aKeys.at(i));
		this->disconnect(pCC);
		pCC->disconnect();
		pCC->deleteLater();

	} // loop

	this->hConnections.clear();

	qApp->quit();

} // quit


void SwissalpS::QtSssSircBot::AppController::reloadConnections() {

	// first disconnect all existing connections
	QStringList aKeys = this->hConnections.keys();
	IRCclientController *pCC;
	for (int i = 0; i < aKeys.count(); ++i) {

		pCC = this->hConnections.value(aKeys.at(i));
		this->disconnect(pCC);
		pCC->disconnect();
		pCC->deleteLater();

	} // loop

	this->hConnections.clear();

	this->initConnections();

} // reloadConnections


void AppController::run() {

	std::cout << "run" << std::endl;

	// init settings
	this->initSettings();

	QDir::setCurrent(this->pAS->getPathData());

	this->writePID();

	// init 'error' messaging system to route all through app controller
	this->connectErrorMessages();

	this->initConnections();

	this->initFileCommandInterface();

} // run


void AppController::writePID() {

	QString sPathPID = AppSettings::pAppSettings()->get(AppSettings::sSettingPIDpathFile).toString();
	QString sPID = QString::number(QCoreApplication::applicationPid());

	QFile oPID(sPathPID);
	if (!oPID.open(QIODevice::WriteOnly)) {

		std::cout << (tr("AppController::KO:Cannot write to pid-file: ")
					  + sPathPID).toStdString();
		std::cout << std::endl;

		return;

	} // if failed to open pid-file

	oPID.write(sPID.toUtf8());
	oPID.close();

} // writePID



}	} // namespace SwissalpS::QtSssSircBot
