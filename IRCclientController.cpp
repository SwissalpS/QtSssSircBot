#include "IRCclientController.h"
// TODO: make this class independant from AppSettings
#include "AppSettings.h"

#include <QJsonArray>
#include <QTimer>



namespace SwissalpS { namespace QtSssSircBot {



IRCclientController::IRCclientController(const QJsonObject &oConfig,
										 QObject *pParent) :
	QObject(pParent),
	pClient(nullptr) {

	// TODO: make sure all keys are set with some value
	this->oJo = QJsonObject(oConfig);
/*
	if (oConfig.contains(AppSettings::sSettingIRCremoteHost)) {
		this->oJo.insert(AppSettings::sSettingIRCremoteHost,
						 oConfig.value(AppSettings::sSettingIRCremoteHost));
	}
	if (oConfig.contains(AppSettings::sSettingIRCremoteNick)) {
		this->oJo.insert(AppSettings::sSettingIRCremoteNick,
						 oConfig.value(AppSettings::sSettingIRCremoteNick));
	}
	if (oConfig.contains(AppSettings::sSettingIRCremotePort)) {
		this->oJo.insert(AppSettings::sSettingIRCremotePort,
						 oConfig.value(AppSettings::sSettingIRCremotePort));
	}
	if (oConfig.contains(AppSettings::sSettingIRCremoteRealname)) {
		this->oJo.insert(AppSettings::sSettingIRCremoteRealname,
						 oConfig.value(AppSettings::sSettingIRCremoteRealname));
	}
	if (oConfig.contains(AppSettings::sSettingIRCremotePassword)) {
		this->oJo.insert(AppSettings::sSettingIRCremotePassword,
						 oConfig.value(AppSettings::sSettingIRCremotePassword));
	}
	if (oConfig.contains(AppSettings::sSettingIRCremoteChannels)) {
		this->oJo.insert(AppSettings::sSettingIRCremoteChannels,
						 oConfig.value(AppSettings::sSettingIRCremoteChannels));
	}
	if (oConfig.contains("sIRCrawPostLoginLines")) {
		this->oJo.insert("sIRCrawPostLoginLines",
						 oConfig.value("sIRCrawPostLoginLines"));
	}
	if (oConfig.contains("sIRCrawPreLoginLines")) {
		this->oJo.insert("sIRCrawPreLoginLines",
						 oConfig.value("sIRCrawPreLoginLines"));
	}
*/
} // construct


IRCclientController::~IRCclientController() {

	if (this->pClient) {
		this->disconnect(this->pClient);
		this->pClient->sendQuit();
		this->pClient->disconnectSocket();
		this->pClient->deleteLater();
	}

	this->pClient = nullptr;

} // dealloc


QString IRCclientController::getConnectionID() {

	return this->oJo.value("sConnectionID").toString();

} // getConnectionID


void IRCclientController::init() {

	this->pClient = new IRCclient(
						this->oJo.value(AppSettings::sSettingIRCremoteHost).toString(),
						this->oJo.value(AppSettings::sSettingIRCremoteNick).toString(),
						this->oJo.value(AppSettings::sSettingIRCremotePort).toInt(),
						this->oJo.value(AppSettings::sSettingIRCremoteRealname).toString(),
						this->oJo.value(AppSettings::sSettingIRCremotePassword).toString(),
						this);

	connect(this->pClient, SIGNAL(abort(qint16)),
			this, SLOT(onAbort(qint16)));

	connect(this->pClient, SIGNAL(connected(QString)),
			this, SLOT(onConnected(QString)));

	connect(this->pClient, SIGNAL(channelMessage(QString, QString, QString)),
			this, SLOT(onChannelMessage(QString, QString, QString)));

	connect(this->pClient, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	connect(this->pClient, SIGNAL(directMessage(QString, QString)),
			this, SLOT(onDirectMessage(QString, QString)));

	connect(this->pClient, SIGNAL(disconnected()),
			this, SLOT(onDisconnected()));

	connect(this->pClient, SIGNAL(joined(QString, QString)),
			this, SLOT(onJoined(QString, QString)));

	connect(this->pClient, SIGNAL(loggedIn(QString)),
			this, SLOT(onLoggedIn(QString)));

	connect(this->pClient, SIGNAL(nicklist(QString, QStringList)),
			this, SLOT(onNicklist(QString, QStringList)));

	connect(this->pClient, SIGNAL(ping(QString)),
			this, SLOT(onPing(QString)));

	connect(this->pClient, SIGNAL(quit(QString, QString)),
			this, SLOT(onQuit(QString, QString)));

	connect(this->pClient, SIGNAL(rawIncomingLine(QString)),
			this, SLOT(onRawIncomingLine(QString)));

	connect(this->pClient, SIGNAL(rawOutgoingLine(QString)),
			this, SLOT(onRawOutgoingLine(QString)));

} // init


void IRCclientController::start() {

	if (!this->pClient) this->init();

	this->pClient->connectEncrypted();

} // start


void IRCclientController::onAbort(const qint16 &iR) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::Abort))
						  << QString::number(iR));

} // onAbort


void IRCclientController::onConnected(const QString &sIP) {
	Q_UNUSED(sIP)

} // onConnected


void IRCclientController::onChannelMessage(const QString &sChannel,
										   const QString &sFromNick,
										   const QString &sMessage) {
	Q_UNUSED(sChannel)
	Q_UNUSED(sFromNick)
	Q_UNUSED(sMessage)

} // onChannelMessage


void IRCclientController::onDirectMessage(const QString &sFromNick,
										  const QString &sMessage) {
	Q_UNUSED(sFromNick)
	Q_UNUSED(sMessage)

} // onDirectMessage


void IRCclientController::onDisconnected() {

} // onDisconnected


void IRCclientController::onJoined(const QString &sNick,
								   const QString &sChannel) {
	Q_UNUSED(sNick)
	Q_UNUSED(sChannel)

} // onJoined


void IRCclientController::onLoggedIn(const QString &sNick) {
	Q_UNUSED(sNick)

	const QJsonArray aChannels = this->oJo.value(AppSettings::sSettingIRCremoteChannels).toArray();

	for (int i = 0; i < aChannels.count(); ++i) {

		this->pClient->sendJoin(aChannels.at(i).toString());

	} // loop

	const QJsonArray aChannels = this->oJo.value(AppSettings::sSettingIRCremoteChannels).toArray();

	for (int i = 0; i < aChannels.count(); ++i) {

		this->pClient->sendJoin(aChannels.at(i).toString());

	} // loop

} // onLoggedIn


void IRCclientController::onNicklist(const QString &sChannel,
									 const QStringList &aNicks) {
	Q_UNUSED(sChannel)
	Q_UNUSED(aNicks)

} // onNicklist


void IRCclientController::onPing(const QString &sMessage) {
	Q_UNUSED(sMessage)

} // onPing


void IRCclientController::onQuit(const QString &sNick, const QString &sMessage) {
	Q_UNUSED(sNick)
	Q_UNUSED(sMessage)

} // onQuit


void IRCclientController::onRawIncomingLine(const QString &sLine) {
	Q_UNUSED(sLine)

} // onRawIncomingLine


void IRCclientController::onRawOutgoingLine(const QString &sLine) {
	Q_UNUSED(sLine)

} // onRawOutgoingLine



}	} // namespace SwissalpS::QtSssSircBot
