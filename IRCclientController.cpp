#include "IRCclientController.h"
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


// AppController asks for this
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


// AppController triggers this
void IRCclientController::start() {

	if (!this->pClient) this->init();

	this->pClient->connectEncrypted();

} // start


// signal from IRCclient
void IRCclientController::onAbort(const qint16 &iR) {
	Q_UNUSED(iR)

} // onAbort


// signal from IRCclient
void IRCclientController::onConnected(const QString &sIP) {
	Q_UNUSED(sIP)

	this->iLineIndex = 0;
	this->onPreLoginNextLine();

} // onConnected


// signal from IRCclient
void IRCclientController::onChannelMessage(const QString &sChannel,
										   const QString &sFromNick,
										   const QString &sMessage) {
	Q_UNUSED(sChannel)
	Q_UNUSED(sFromNick)
	Q_UNUSED(sMessage)

} // onChannelMessage


// signal from IRCclient
void IRCclientController::onDirectMessage(const QString &sFromNick,
										  const QString &sMessage) {
	Q_UNUSED(sFromNick)
	Q_UNUSED(sMessage)

} // onDirectMessage


// signal from IRCclient
void IRCclientController::onDisconnected() {

} // onDisconnected


// signal from IRCclient
void IRCclientController::onJoined(const QString &sNick,
								   const QString &sChannel) {
	Q_UNUSED(sNick)
	Q_UNUSED(sChannel)

} // onJoined


// used after logged in (protected slot)
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


// signal from IRCclient
void IRCclientController::onLoggedIn(const QString &sNick) {
	Q_UNUSED(sNick)

	this->iLineIndex = 0;
	this->onPostLoginNextLine();

} // onLoggedIn


// signal from IRCclient
void IRCclientController::onNicklist(const QString &sChannel,
									 const QStringList &aNicks) {
	Q_UNUSED(sChannel)
	Q_UNUSED(aNicks)

} // onNicklist


// signal from IRCclient
void IRCclientController::onPing(const QString &sMessage) {
	Q_UNUSED(sMessage)

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


// signal from IRCclient
void IRCclientController::onQuit(const QString &sNick, const QString &sMessage) {
	Q_UNUSED(sNick)
	Q_UNUSED(sMessage)

} // onQuit


// signal from IRCclient
void IRCclientController::onRawIncomingLine(const QString &sLine) {
	Q_UNUSED(sLine)

} // onRawIncomingLine


// signal from IRCclient
void IRCclientController::onRawOutgoingLine(const QString &sLine) {
	Q_UNUSED(sLine)

} // onRawOutgoingLine



}	} // namespace SwissalpS::QtSssSircBot
