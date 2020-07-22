#include "IRCclientController.h"
#include "IRCeventCodes.h"
#include "AppSettings.h"
#include <QJsonArray>



namespace SwissalpS { namespace QtSssSircBot {



IRCclientController::IRCclientController(const QJsonObject &oConfig,
										 QObject *pParent) :
	QObject(pParent),
	pClient(nullptr) {

	// TODO: should not need to know about AppSettings class at all
	AppSettings *pAS = AppSettings::pAppSettings();
	this->oJo = QJsonObject();
	this->oJo.insert("sConnectionID", "unset");
	this->oJo.insert(AppSettings::sSettingIRCremoteHost,
					 pAS->get(AppSettings::sSettingIRCremoteHost).toString());
	this->oJo.insert(AppSettings::sSettingIRCremoteNick,
					 pAS->get(AppSettings::sSettingIRCremoteNick).toString());
	this->oJo.insert(AppSettings::sSettingIRCremotePort,
					 (double)pAS->get(AppSettings::sSettingIRCremotePort).toUInt());
	this->oJo.insert(AppSettings::sSettingIRCremoteRealname,
					 pAS->get(AppSettings::sSettingIRCremoteRealname).toString());
	this->oJo.insert(AppSettings::sSettingIRCremotePassword,
					 pAS->get(AppSettings::sSettingIRCremotePassword).toString());
	QJsonArray oJa;
	const QStringList aChannels = pAS->getChannels();
	for (int i = 0; i < aChannels.count(); ++i) oJa.append(aChannels.at(i));
	this->oJo.insert(AppSettings::sSettingIRCremoteChannels, oJa);

	this->oJo.insert("sIRCrawPostLoginLines", QJsonArray());
	this->oJo.insert("sIRCrawPreLoginLines", QJsonArray());

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

} // construct


IRCclientController::~IRCclientController() {

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

	connect(this->pClient, SIGNAL(quit(qint16)),
			this, SLOT(onQuit(qint16)));

	connect(this->pClient, SIGNAL(rawIncomingLine(QString)),
			this, SLOT(onRawIncomingLine(QString)));

	connect(this->pClient, SIGNAL(rawOutgoingLine(QString)),
			this, SLOT(onRawOutgoingLine(QString)));

} // init


void IRCclientController::start() {

	if (!this->pClient) this->init();

	this->pClient->connectEncrypted();

} // start


void IRCclientController::onConnected(const QString &sIP) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::Connected)) << sIP);

} // onConnected


void IRCclientController::onChannelMessage(const QString &sChannel,
										   const QString &sFromNick,
										   const QString &sMessage) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::ChannelMessage))
						  << sChannel << sFromNick << sMessage);

} // onChannelMessage


void IRCclientController::onDirectMessage(const QString &sFromNick,
										  const QString &sMessage) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::DirectMessage))
						  << sFromNick << sMessage);

} // onDirectMessage


void IRCclientController::onDisconnected() {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::Disconnected)));

} // onDisconnected


void IRCclientController::onJoined(const QString &sNick,
								   const QString &sChannel) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::Joined))
						  << sNick << sChannel);

} // onJoined


void IRCclientController::onLoggedIn(const QString &sNick) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::LoggedIn))
						  << sNick);

} // onLoggedIn


void IRCclientController::onNicklist(const QString &sChannel,
									 const QStringList &aNicks) {

	QStringList aEvent = QStringList() << this->getConnectionID()
									   << QString(QChar(IRCeventCodes::NickList))
									   << sChannel;
	aEvent.append(aNicks);

	Q_EMIT this->newEvent(aEvent);

} // onNicklist


void IRCclientController::onPing(const QString &sMessage) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::Ping))
						  << sMessage);

} // onPing


void IRCclientController::onQuit(const qint16 iR) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::Quit))
						  << QString::number(iR));

} // onQuit


void IRCclientController::onRawIncomingLine(const QString &sLine) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::RawIn))
						  << sLine);

} // onRawIncomingLine


void IRCclientController::onRawOutgoingLine(const QString &sLine) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(IRCeventCodes::RawOut))
						  << sLine);

} // onRawOutgoingLine



}	} // namespace SwissalpS::QtSssSircBot
