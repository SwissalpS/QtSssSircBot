#include "IRCclientController.h"
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

	connect(this->pClient, SIGNAL(channelMessage(QString, QString)),
			this, SLOT(onChannelMessage(QString, QString)));

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



}	} // namespace SwissalpS::QtSssSircBot
