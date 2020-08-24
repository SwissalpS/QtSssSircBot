#include "IRCclientController.h"
#include "CommandEventCodes.h"
// TODO: make this class independant from AppSettings
#include "AppSettings.h"

#include <QJsonArray>
#include <QTimer>



namespace SwissalpS { namespace QtSssSircBot {



IRCclientController::IRCclientController(const QJsonObject &oConfig,
										 QObject *pParent) :
	QObject(pParent),
	pClient(nullptr),
	iChannelIndex(0),
	iLineIndex(0) {

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


/// AppController asks for this
QString IRCclientController::getConnectionID() {

	return this->oJo.value("sConnectionID").toString();

} // getConnectionID


void IRCclientController::init() {

	QStringList aNicks;
	QJsonArray oJa = this->oJo.value(AppSettings::sSettingIRCremoteNicks).toArray();
	for (int i = 0; i < oJa.count(); ++i) aNicks << oJa.at(i).toString();

	this->pClient = new IRCclient(
						this->oJo.value(AppSettings::sSettingIRCremoteHost).toString(),
						aNicks,
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


/// AppController triggers this
void IRCclientController::start() {

	if (!this->pClient) this->init();

	this->pClient->connectEncrypted();

} // start


/// signal from IRCclient
void IRCclientController::onAbort(const qint16 &iR) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::Abort))
						  << QString::number(iR));

} // onAbort


/// signal from external command/lua via AppController
void IRCclientController::onCommandEvent(const QStringList &aEvent) {

	// { <connection id>, <interface-command-code>, <parameter0> ... <parameterN> }
	const int iEvent = aEvent.count();

	if (3 > iEvent) {
		this->onDebugMessage("OO:got command event with less than 3 entries.");
		// nothing to do
		return;
	} // if arg count check failed

	const QString sID = aEvent.first();
	if (0 != sID.compare(this->getConnectionID())) {
		// not for this connection
		return;
	}

	const quint8 ubCC = aEvent.at(1).at(0).unicode();

	int i;
	QStringList aParams;
	switch (ubCC) {
		case CommandEventCodes::Abort:
			// kill Lua instance -> handled by AppController

		break;
		case CommandEventCodes::ChannelMessage:

			if (4 > iEvent) {
				this->onDebugMessage("OO:too few arguments given");
				return;
			}
			this->pClient->sendPrivateMessage(aEvent.at(2), aEvent.at(3));
		break;
		case CommandEventCodes::Connected:
			QTimer::singleShot(0, this->pClient, SLOT(reconnect()));
		break;
		case CommandEventCodes::DirectMessage:

			if (4 > iEvent) {
				this->onDebugMessage("OO:too few arguments given");
				return;
			}
			this->pClient->sendPrivateMessage(aEvent.at(2), aEvent.at(3));
		break;
		case CommandEventCodes::Disconnected:
			QTimer::singleShot(0, this->pClient, SLOT(disconnectSocket()));
		break;
		case CommandEventCodes::IRCcommand:
			for (i = 3; i < iEvent; ++i) aParams.append(aEvent.at(i));
			this->pClient->sendIRCCommand(aEvent.at(2), aParams);
		break;
		case CommandEventCodes::Quit:
			this->pClient->sendQuit(aEvent.at(2));
		break;
		case CommandEventCodes::Joined:
			this->pClient->sendJoin(aEvent.at(2));
		break;
		case CommandEventCodes::NickList:
			this->pClient->sendNicknameChangeRequest(aEvent.at(2));
		break;
		case CommandEventCodes::Part:
			if (4 <= iEvent) {
				this->pClient->sendPart(aEvent.at(2), aEvent.at(3));
			} else {
				this->pClient->sendPart(aEvent.at(2));
			}
		break;

		default:
			this->onDebugMessage("unknown interface-command-code given: "
								 + QString::number(ubCC) + " " + QChar(ubCC));
		break;

	} // switch ubCC

} // onCommandEvent


/// signal from IRCclient
void IRCclientController::onConnected(const QString &sIP) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::Connected)) << sIP);

	this->iLineIndex = 0;
	this->onPreLoginNextLine();

} // onConnected


/// signal from IRCclient
void IRCclientController::onChannelMessage(const QString &sChannel,
										   const QString &sFromNick,
										   const QString &sMessage) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::ChannelMessage))
						  << sChannel << sFromNick << sMessage);

} // onChannelMessage


/// signal from IRCclient
void IRCclientController::onDirectMessage(const QString &sFromNick,
										  const QString &sMessage) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::DirectMessage))
						  << sFromNick << sMessage);

} // onDirectMessage


/// signal from IRCclient
void IRCclientController::onDisconnected() {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::Disconnected)));

} // onDisconnected


/// signal from IRCclient
void IRCclientController::onJoined(const QString &sNick,
								   const QString &sChannel) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::Joined))
						  << sNick << sChannel);

} // onJoined


/// used after logged in (protected slot)
void IRCclientController::onJoinNextChannel() {

	const QJsonArray aChannels = this->oJo.value(AppSettings::sSettingIRCremoteChannels).toArray();

	int iTotal = aChannels.count();
	if (this->iChannelIndex >= iTotal) return;

	this->pClient->sendJoin(aChannels.at(this->iChannelIndex).toString());

	this->iChannelIndex++;
	if (this->iChannelIndex >= iTotal) return;

	// TODO: use a setting for delay
	QTimer::singleShot(1234, this, SLOT(onJoinNextChannel()));

} // onJoinNextChannel


/// signal from IRCclient
void IRCclientController::onLoggedIn(const QString &sNick) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::LoggedIn))
						  << sNick);

	this->iLineIndex = 0;
	this->onPostLoginNextLine();

} // onLoggedIn


/// signal from lua via irc-api and AppController
/// depricated: use onCommandEvent() instead
void IRCclientController::onLuaEvent(const QStringList &aEvent) {

	this->onDebugMessage("OO:depricated call to onLuaEvent!");

	return this->onCommandEvent(aEvent);

} // onLuaEvent


/// signal from IRCclient
void IRCclientController::onNicklist(const QString &sChannel,
									 const QStringList &aNicks) {

	QStringList aEvent = QStringList() << this->getConnectionID()
									   << QString(QChar(CommandEventCodes::NickList))
									   << sChannel;
	aEvent.append(aNicks);

	Q_EMIT this->newEvent(aEvent);

} // onNicklist


/// signal from IRCclient
void IRCclientController::onPing(const QString &sMessage) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::Ping))
						  << sMessage);

} // onPing


void IRCclientController::onPostLoginNextLine() {

	const QJsonArray aLines = this->oJo.value("sIRCrawPostLoginLines").toArray();

	int iTotal = aLines.count();
	if (this->iLineIndex < iTotal) {

		this->pClient->sendLine(aLines.at(this->iLineIndex).toString());

		this->iLineIndex++;
		if (this->iLineIndex < iTotal) {

			QTimer::singleShot(1234, this, SLOT(onPostLoginNextLine()));
			return;

		} // if has more lines

	} // if has lines

	// time to start joining channels
	this->iChannelIndex = 0;
	this->onJoinNextChannel();

} // onPostLoginNextLine


void IRCclientController::onPreLoginNextLine() {

	const QJsonArray aLines = this->oJo.value("sIRCrawPreLoginLines").toArray();

	int iTotal = aLines.count();
	if (this->iLineIndex < iTotal) {

		this->pClient->sendLine(aLines.at(this->iLineIndex).toString());

		this->iLineIndex++;
		if (this->iLineIndex < iTotal) {

			QTimer::singleShot(1234, this, SLOT(onPreLoginNextLine()));
			return;

		} // if has more lines

	} // if has lines

	this->pClient->doLogin();

} // onPreLoginNextLine


/// signal from IRCclient
void IRCclientController::onQuit(const QString &sNick, const QString &sMessage) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::Quit))
						  << sNick << sMessage);

} // onQuit


/// signal from IRCclient
void IRCclientController::onRawIncomingLine(const QString &sLine) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::RawIn))
						  << sLine);

} // onRawIncomingLine


/// signal from IRCclient
void IRCclientController::onRawOutgoingLine(const QString &sLine) {

	Q_EMIT this->newEvent(QStringList() << this->getConnectionID()
						  << QString(QChar(CommandEventCodes::RawOut))
						  << sLine);

} // onRawOutgoingLine



}	} // namespace SwissalpS::QtSssSircBot

